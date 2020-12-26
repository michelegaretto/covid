/*
**  File:        virus.h
**  Program:     virus
**  Purpose:     virtual class Virus 
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#ifndef virus_h
#define virus_h

#include <stdio.h>
#include "pos.h"
#include "posevent.h"
#include "scheduler.h"
#include "service.h"

#define MAXPOS 5000000L
#define MAXBINS 10000L


class Simulation;

class Virus : public Handler {

friend class Simulation;

public:

	Virus(Simulation* s) : fr_(stdout), fl_(stdout), ft_(stdout), s_(s), computeR0_(0) { }
	
	inline long cumpos() { return cum_pos_; }
		
	inline void attachfileres(FILE * fr) { fr_ = fr; }
	inline void attachfilelog(FILE * fl) { fl_ = fl; }	
	inline void attachfiletrace(FILE * ft) { ft_ = ft; }
			
	virtual void inputparameters();	
	virtual void init();
	virtual void reset();
	virtual void finish();
	void test_tri();
			
	
protected:	
	
	FILE* fr_;		/* output file for results */ 
	FILE* fl_;		/* output file for log */
	FILE* ft_;		/* output file for trace */
	int debug_;
	
	Simulation* s_;	        /* pointer to simulation object */
		
	/* main data structures */
	unsigned long int numfreeid_;
	unsigned long int freeid_[MAXPOS+1];
	unsigned long int incubid_[MAXPOS+1];
	unsigned long int presympid_[MAXPOS+1];
	unsigned long int superid_[MAXPOS+1];
	unsigned long int asympid_[MAXPOS+1];
	unsigned long int residualid_[MAXPOS+1];
	
	Pos* pos_[MAXPOS+1];
	int todeath_[MAXPOS+1];
	int intensive_[MAXPOS+1];
	
	/* parameters */
	double minincubtime_;
	double maxincubtime_;
	double aveincubtime_;
	double peakincubtime_;
	int typeincubtime_; // uniform, triangular
	
	double presicktime_; // constant
	double minsicktime_;
	double maxsicktime_;
	double avesicktime_;
	double peaksicktime_;
	int typesicktime_; // uniform, triangular
	
	double mindeathtime_;
	double maxdeathtime_;
	double avedeathtime_;
	double peakdeathtime_;
	int typedeathtime_; // uniform, triangular
	
	double averesidualtime_;
	double sigmaresidualtime_;
	int typeresidualtime_; // exp, lognormal
		
	double lowrate_;
	double highrate_;
	
	double pasymp_; // asymptomatic probability
	double psuper_; // super diffusor probability
	double psymp_;  // symptomatic probability
	double pdeath_; // probability to go into intensive therapy followed by death
	double pintensive_; // probability to go into intensive therapy followed by recover
	
	long initialpos_; // initial number of positives
	int typeoffactor_; // factor due to mobility restrictions 
	double factor_param1_; 
	double factor_param2_; 
	double factor_param3_; 
	double factor_param4_; 
	double factor_param5_; 
	double factor_param6_; 
	double factor_param7_; 
	int typeofswabs_; // swabs rate function
	double swabs_param1_;
	double swabs_param2_; 
	double swabs_param3_; 
	double anticipate_;
			
	/* variables */
	long num_pos_;
	long cum_pos_;
	long num_u_incub_;    
	long num_k_incub_;
	long num_u_pre_symp_;
	long num_k_pre_symp_;
	long num_u_asymp_;
	long num_k_asymp_;
	long num_u_super_;
	long num_k_super_;
	long num_u_symp_;
	long num_k_symp_;
	long num_u_recovered_;
	long num_k_recovered_;
	long num_u_dead_;
	long num_k_dead_;
	long num_u_residual_;
	long num_k_residual_;	
	long num_u_intensive_;
	long num_k_intensive_;
	
	long old_num_pos_;
	long old_cum_pos_;
	long old_num_u_incub_;    
	long old_num_k_incub_;
	long old_num_u_pre_symp_;
	long old_num_k_pre_symp_;
	long old_num_u_asymp_;
	long old_num_k_asymp_;
	long old_num_u_super_;
	long old_num_k_super_;
	long old_num_u_symp_;
	long old_num_k_symp_;
	long old_num_u_recovered_;
	long old_num_k_recovered_;
	long old_num_u_dead_;
	long old_num_k_dead_;
	long old_num_u_residual_;
	long old_num_k_residual_;
	long old_num_u_intensive_;
	long old_num_k_intensive_;
	
	/* methods */
	double genincub() { if(typeincubtime_ == 0) return RNG::uniform(minincubtime_,maxincubtime_); else return RNG::triangular(minincubtime_,maxincubtime_,peakincubtime_); }
	double gensick() { if(typesicktime_ == 0) return RNG::uniform(minsicktime_,maxsicktime_); else return RNG::triangular(minsicktime_,maxsicktime_,peaksicktime_); }
	double gendeath() { if(typedeathtime_ == 0) return RNG::uniform(mindeathtime_,maxdeathtime_); else return RNG::triangular(mindeathtime_,maxdeathtime_,peakdeathtime_); }
	double genresidual() { if(typeresidualtime_ == 0) return RNG::exponential(averesidualtime_); else return RNG::lognormal(averesidualtime_,sigmaresidualtime_); }
	void handle(Event *e);
	void newinfection();
	void handle_startpresymp(long id);
	void handle_endpresymp(long id);
	void handle_endsick(long id);
	void handle_endinte(long id);
	void handle_endresidual(long id);
	
	double computefactor(double t);
	double computeswabsrate(double t);
	void sample();
	
	/* statistics */
	void printsituation();
	int computeR0_;
	double sumR0_[MAXBINS];
	long numR0_[MAXBINS];
	double binR0_;
	double guard_;
	double deltaK_[MAXBINS];
	double deltaR_[MAXBINS];
	double sumK_[MAXBINS];
	double oldK_;
	double oldR_;
};


#endif
	
