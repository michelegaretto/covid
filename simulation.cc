/*
**  File:        simulation.cc
**  Program:     corona
**  Purpose:     class Simulation      
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#include <assert.h>
#include <math.h>
#include "simulation.h"
#include "service.h"

	
void BatchTimer::expire(Event *e)
{
	s_->batch();
}

void DurationTimer::expire(Event *e)
{
	s_->finish();
}	

void SampleTimer::expire(Event *e)
{
	s_->sample();
}	

void Simulation::batch()
{
	double now = Scheduler::instance().clock();
		
	batch_++;
	
        batchmeans_[1][batch_] = -1;
        fprintf(stdout,"# Sim: now = %f - batch = %d \n", now, batch_);
		
        if(batch_ < numbatches_) {
		bt_->resched(durbatch_);
	} else {
		analyzebatches();
		finish();
	}		
}

void Simulation::finish()
{
	double now = Scheduler::instance().clock();
	int i;
	
	fprintf(fl_,"# Sim: simulation finished. now = %f\n", now);
	
	printresults();		
	Scheduler::halt();
	
	v_->finish();
	
	fprintf(fl_,"# Exiting...\n");
	fclose(fr_);
	fclose(fl_);	
	fclose(ft_);	
}

void Simulation::startbatch()
{
	batch_ = 0;
	for(int j=0; j <= NUMBATCHES; j++) batchmeans_[1][j] = 0;
		
	fprintf(fl_,"# Sim: first batch scheduled: durbatch = %f\n",durbatch_);
	bt_->sched(durbatch_); 	    
}

void PrintTimer::expire(Event *e)
{
	s_->print();
}	
	
void Simulation::print() 
{
	double now = Scheduler::instance().clock();
	v_->printsituation();
	pt_->resched(delta_print_);
}	

/*
**  Function : sample()
**  Purpose:   basic sampling process  
*/
void Simulation::sample()
{
	v_->sample();
	st_->resched(delta_sample_);	
}	

/*
**  Function : printresults()
**  Purpose:   print out results on file   
**              
*/
void Simulation::printresults()
{	
	double mean,var,min,ave,max;
	mean = var = -1;
	if(durbatch_ > 0) {
        	computeconf(batchmeans_[1], &min, &ave, &max); 
        } else {
	       	min = ave = max = -1;
        }		
        fprintf(fr_,"# mean = %le - var = %le - ave = %le - min = %le - max = %le\n",mean,var,ave,min,max);
        printf("Simulation results: mean = %lf - var = %lf - ave = %lf - min = %lf - max = %lf\n",mean,var,ave,min,max);
}
		
/*
**  Function : inputparamters()
**  Purpose:   Input parameters  
**              
*/
void Simulation::inputparameters()
{ 
	char filename[255];
	
	printf("Run simulation ? (0/1): ");
  	runsim_ = get_int();
  	printf("%d\n",runsim_);
  	if ((runsim_ < 0) || (runsim_ > 1))  error("value not valid");
	
	printf("Name of simulation: ");
  	get_string(name_);
  	printf("%s\n",name_);
	
	printf("Debug level : ");
	debug_ = get_int();
  	printf("%d\n",debug_);
	if (debug_ < 0) error("Debug level not valid !");
	
	printf("Transient duration (s) (0 = none): ");
  	transient_ = get_double();
  	printf("%f\n",transient_);
	rt_ = new TranTimer(this); 

	printf("Batch duration (s) (0 = no batch analysis : -1 = equal to transient): ");
    durbatch_ = get_double();
	if(durbatch_ == -1) durbatch_ = transient_;
    printf("%f\n",durbatch_);
 	if (durbatch_ < 0.) error("Batch duration not valid");
    if (durbatch_ > 0) bt_ = new BatchTimer(this); else dt_ = new DurationTimer(this);
	
	printf("Simulation duration (s) (zero = determined by batch analysis): ");
	duration_ = get_double();
	printf("%f\n",duration_);
	if (duration_ < 0.) error("Duration not valid");
	if (duration_ == 0. && durbatch_ == 0) error("Simulation duration not valid");	
	if (duration_ > 0. && durbatch_ > 0) error("simulation duration is given by batch analysis");
	if (duration_ > 0) duration_ = transient_ + duration_;
	if (durbatch_ > 0.0) duration_ = transient_ + durbatch_*numbatches_;
	
	printf("Sample interval (s) (zero = none): ");
	delta_sample_ = get_double();
	printf("%f\n",delta_sample_);
	if (delta_sample_ > 0.) st_ = new SampleTimer(this); 
        
	printf("Print interval (s) (zero = none): ");
	delta_print_ = get_double();
	printf("%f\n",delta_print_);
	if (delta_print_ > 0.) pt_ = new PrintTimer(this); 
		
	/* trace file for simulation */
	sprintf(filename,"%s%s",name_,".trace");
    ft_ = fopen(filename,"w");
    assert(ft_ != 0);
	fprintf(ft_,"### TRACE FOR SIMULATION '%s' ###\n",name_);
	fprintf(ft_,"\n");
	
	/* result file for simulation */
	sprintf(filename,"%s%s",name_,".res");
    fr_ = fopen(filename,"w");
    assert(fr_ != 0);
	fprintf(fr_,"### RESULTS FOR SIMULATION '%s' ###\n",name_);
	fprintf(fr_,"\n");
	
	/* log file for simulation */
    sprintf(filename,"%s%s",name_,".log");
    fl_ = fopen(filename,"w");
	assert(fl_ != 0);
    fprintf(fl_,"### LOG OF SIMULATION '%s' ###\n",name_);
	fprintf(fl_,"\n");
}

void TranTimer::expire(Event*e)
{
	s_->reset(); 
}	

/*
**  Function : reset()
**  Purpose:   reset at the end of transient  
**              
*/
void Simulation::reset()
{
	double now = Scheduler::instance().clock();
	fprintf(fl_,"# Sim: now = %f - End of transient\n", now);
	if(durbatch_ > 0) startbatch(); 
}		

/*
**  Function : startsim()
**  Purpose:   schedules the first events of the simulation  
**              
*/
void Simulation::startsim()
{	
	v_->attachfileres(fr_);
	v_->attachfiletrace(ft_); 
	v_->attachfilelog(fl_);
	
	if(delta_sample_) st_->sched(delta_sample_);
	if(delta_print_) pt_->sched(delta_print_);
	
	// init virus
	v_->init();
	
	if(debug_ > 2) printf("# Startsim: delta_sample = %lf, delta_print = %lf\n",delta_sample_,delta_print_);
	
	if (transient_ > 0.) {
		rt_->sched(transient_);	
		fprintf(fl_,"# Sim: scheduled end of transient after %f sec\n",transient_);
	}	
	
	if (durbatch_ ==  0. && duration_ > 0) {
		dt_->sched(duration_);	
		fprintf(fl_,"# Sim: scheduled end of simulation after %f sec\n",duration_);
	}	
}	
	
void Simulation::handle(Event *e)
{
	double now = Scheduler::instance().clock();		
	fprintf(fl_,"# %f : Simulation : called handle event %p\n",now, e);
	free(e);					
	error("should not happen");	
}	

