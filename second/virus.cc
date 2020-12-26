/*
**  File:        virus.cc
**  Program:     corona
**  Purpose:     class Virus      
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#include <math.h>
#include "virus.h"
#include "service.h"
#include "scheduler.h"
#include "simulation.h"

#include <gsl/gsl_cdf.h>

// #define PRINTFACTOR
// #define FITGAMMA

#define GIVENDETECTED
#define GIVENREAL

#define MORTALITY 0.01   // 0.02306667;  


/*
**  Function : inputparameters()
**  Purpose:   Input parameters  
**              
*/
void Virus::inputparameters()
{ 
	FILE* fp;
	char riga[1000];
	long t;
	double val;
	
	/* set debug level equal to that of simulation */			
	debug_ = s_->debug();	
	
	printf("type of incub ? (unif 0, tri 1): ");
  	typeincubtime_ = get_int();
  	printf("%d\n",typeincubtime_);
  	if ((typeincubtime_ < 0) || (typeincubtime_ > 1))  error("value not valid");
	
	printf("minincubtime: ");
    minincubtime_ = get_double();
    printf("%f\n",minincubtime_);
 	if (minincubtime_ < 0.) error("value not valid");
	
	printf("maxincubtime: ");
    maxincubtime_ = get_double();
    printf("%f\n",maxincubtime_);
 	if (maxincubtime_ < 0.) error("value not valid");
	
	printf("aveincubtime (if applicable): ");
    aveincubtime_ = get_double();
    printf("%f\n",aveincubtime_);
 	if (typeincubtime_ == 1) {
		peakincubtime_ = 3*aveincubtime_ - maxincubtime_ - minincubtime_;
		if(debug_ > 1) printf("peakincubtime = %lf\n",peakincubtime_);
		if(peakincubtime_ < minincubtime_ || peakincubtime_ > maxincubtime_) error("value not valid");
	}
	
	printf("presicktime: ");
    presicktime_ = get_double();
    printf("%f\n",presicktime_);
 	if (presicktime_ < 0) error("value not valid");
	
	printf("type of sick time ? (unif 0, tri 1): ");
  	typesicktime_ = get_int();
  	printf("%d\n",typesicktime_);
  	if ((typesicktime_ < 0) || (typesicktime_ > 1)) error("value not valid");
	
	printf("minsicktime: ");
    minsicktime_ = get_double();
    printf("%f\n",minsicktime_);
 	if (minsicktime_ < 0.) error("value not valid");
	
	printf("maxsicktime: ");
    maxsicktime_ = get_double();
    printf("%f\n",maxsicktime_);
 	if (maxsicktime_ < 0.) error("value not valid");
	
	printf("avesicktime (if applicable): ");
    avesicktime_ = get_double();
    printf("%f\n",avesicktime_);
 	if (typesicktime_ == 1) {
		peaksicktime_ = 3*avesicktime_ - maxsicktime_ - minsicktime_;
		if(debug_ > 1) printf("peaksicktime = %lf\n",peaksicktime_);
		if(peaksicktime_ < minsicktime_ || peaksicktime_ > maxsicktime_) error("value not valid");
	}
	
	printf("type of death time ? (unif 0, tri 1): ");
  	typedeathtime_ = get_int();
  	printf("%d\n",typedeathtime_);
  	if ((typedeathtime_ < 0) || (typedeathtime_ > 1)) error("value not valid");
	
	printf("mindeathtime: ");
    mindeathtime_ = get_double();
    printf("%f\n",mindeathtime_);
 	if (mindeathtime_ < 0.) error("value not valid");
	
	printf("maxdeathtime: ");
    maxdeathtime_ = get_double();
    printf("%f\n",maxdeathtime_);
 	if (maxdeathtime_ < 0.) error("value not valid");
	
	printf("avedeathtime (if applicable): ");
    avedeathtime_ = get_double();
    printf("%f\n",avedeathtime_);
 	if (typedeathtime_ == 1) {
		peakdeathtime_ = 3*avedeathtime_ - maxdeathtime_ - mindeathtime_;
		if(debug_ > 1) printf("peakdeathtime = %lf\n",peakdeathtime_);
		if(peakdeathtime_ < mindeathtime_ || peakdeathtime_ > maxdeathtime_) error("value not valid");
	}
		
	printf("type of residual ? (exp 0, lognorm 1): ");
  	typeresidualtime_ = get_int();
  	printf("%d\n",typeresidualtime_);
  	if ((typeresidualtime_ < 0) || (typeresidualtime_ > 1))  error("value not valid");
	
	printf("averesidualtime: ");
    averesidualtime_ = get_double();
    printf("%f\n",averesidualtime_);
 	if (averesidualtime_ < 0.) error("value not valid");
	
	printf("sigmaresidualtime: ");
    sigmaresidualtime_ = get_double();
    printf("%f\n",sigmaresidualtime_);
 	if (typeresidualtime_ == 1 && sigmaresidualtime_ < 1e-9) error("value not valid");
	
	printf("low infection rate: ");
    lowrate_ = get_double();
    printf("%f\n",lowrate_);
 	if (lowrate_ < 0.) error("value not valid");
	
	printf("high infection rate: ");
    highrate_ = get_double();
    printf("%f\n",highrate_);
 	if (highrate_ < 0.) error("value not valid");
	
	printf("asymptomatic probability: ");
    pasymp_ = get_double();
    printf("%f\n",pasymp_);
 	if (pasymp_ < 0. || pasymp_ > 1) error("value not valid");
	
	printf("super-diffusor probability: ");
    psuper_ = get_double();
    printf("%f\n",psuper_);
 	if (psuper_ < 0. || psuper_ > 1) error("value not valid"); 
	
	psymp_ = 1.0-pasymp_-psuper_;
	if (psymp_ < 0. || psymp_ > 1) error("value not valid");
	
	printf("intensive therapy and death prob: ");
    pdeath_ = get_double();
    printf("%f\n",pdeath_);
 	if (pdeath_ < 0. || pdeath_ > 1) error("value not valid");
 	
 	printf("intensive therapy and recover prob: ");
    pintensive_ = get_double();
    printf("%f\n",pintensive_);
 	if (pintensive_ < 0. || pintensive_ > 1) error("value not valid");
	
	printf("initial number of positives: ");
  	initialpos_ = get_long();
  	printf("%ld\n",initialpos_);
  	if ((initialpos_ < 1) || (initialpos_ > MAXPOS))  error("value not valid");
	
	printf("type of factor: ");
  	typeoffactor_ = get_int();
  	printf("%d\n",typeoffactor_);
  	if ((typeoffactor_ < 1) || (typeoffactor_ > 4))  error("value not valid");
	
	printf("factor param1: ");
    factor_param1_ = get_double();
    printf("%f\n",factor_param1_);
	
	printf("factor param2: ");
    factor_param2_ = get_double();
    printf("%f\n",factor_param2_);
    
    printf("factor param3: ");
    factor_param3_ = get_double();
    printf("%f\n",factor_param3_);
    
    printf("factor param4: ");
    factor_param4_ = get_double();
    printf("%f\n",factor_param4_);
    
    printf("factor param5: ");
    factor_param5_ = get_double();
    printf("%f\n",factor_param5_);
    
    printf("factor param6: ");
    factor_param6_ = get_double();
    printf("%f\n",factor_param6_);
    
    printf("factor param7: ");
    factor_param7_ = get_double();
    printf("%f\n",factor_param7_);
 		
	printf("type of swabs: ");
  	typeofswabs_ = get_int();
  	printf("%d\n",typeofswabs_);
  	if ((typeofswabs_ < 1) || (typeofswabs_ > 3))  error("value not valid");
	
	printf("swabs param1: ");
    swabs_param1_ = get_double();
    printf("%f\n",swabs_param1_);
	
	printf("swabs param2: ");
    swabs_param2_ = get_double();
    printf("%f\n",swabs_param2_);
    
    printf("swabs param3: ");
    swabs_param3_ = get_double();
    printf("%f\n",swabs_param3_);
        
    printf("computeR0 (0/1): ");
  	computeR0_ = get_int();
  	printf("%d\n",computeR0_);
  	if ((computeR0_ < 0) || (computeR0_ > 1))  error("value not valid");    
  	
  	printf("binR0: ");
  	binR0_ = get_double();
    printf("%f\n",binR0_);
 	if (binR0_ < 1e-6) error("value not valid");
 	
 	printf("guard time to compute R0: ");
  	guard_ = get_double();
    printf("%f\n",guard_);
 	if (guard_ < 0) guard_ = maxincubtime_+maxdeathtime_+3*averesidualtime_;
 	
 	printf("anticipation time: ");
  	anticipate_ = get_double();
    printf("%f\n",anticipate_);
 
#ifdef GIVENDETECTED
	/* read deaths */
	fp = fopen("detected.res","r");
	for(t = 0; t < MAXRHO; t++) {
		givendetected_[t] = 0.0;
		determtoq_[t] = 0;
	}	
	while(fgets(riga,1000,fp) != 0) {
		if(sscanf(riga,"%ld %lf",&t,&val) == 2) {
			givendetected_[t] = val; 
			// printf("Letto givendetected_[%d] = %lf\n",t,val);
		}
	}		
	fclose(fp);
#endif 

#ifdef GIVENREAL
	/* read real */
	fp = fopen("cumreal.res","r");
	for(t = 0; t < MAXRHO; t++) {
		givenreal_[t] = 0.0;
	}	
	while(fgets(riga,1000,fp) != 0) {
		if(sscanf(riga,"%ld %*f %lf",&t,&val) == 2) {
			givenreal_[t] = val; 
			if(debug_>10) printf("Letto givenreal_[%ld] = %lf\n",t,val);	
		}
	}		
	fclose(fp);
	// exit(1);
#endif 
 
    
#ifdef PRINTFACTOR
	fp = fopen("factor.res","w");
	for(double t=0; t<=200;t+=0.01)
		fprintf(fp,"%lf \t %lf\n",t-anticipate_,computefactor(t));
	fclose(fp);
	exit(1);
#endif	    

#ifdef FITGAMMA
	
#ifndef SKIP
	// fit of gamma distribution
	double a,b,besta,bestb;
	double err, minerror = 1e6;
	double t1,t2,t3;
	double complete[14000];
	double sum;
	int i,j,z;
	double amplification = 1.0/MORTALITY;
	
	besta = 1.65;
	bestb = 8.45;
	for(a = 1; a<=4; a+=0.01) for(b=3; b<=9; b+=0.01) {
	// if(1) {
		// a = 3; b = 4;
		t1 = gsl_cdf_gamma_Pinv(0.25,a,b);
		t2 = gsl_cdf_gamma_Pinv(0.5,a,b);
		t3 = gsl_cdf_gamma_Qinv(0.25,a,b);
		err = fabs(t1-6) + fabs(t2-12) + fabs(t3-19);
		if(err < minerror) {
			minerror = err;
			besta = a;
			bestb = b;
		}
	}
	printf("besta = %lf - bestb = %lf\n",besta,bestb);
	printf("t1 = %lf\n",gsl_cdf_gamma_Pinv(0.25,besta,bestb));
	printf("t2 = %lf\n",gsl_cdf_gamma_Pinv(0.5,besta,bestb));
	printf("t3 = %lf\n",gsl_cdf_gamma_Qinv(0.25,besta,bestb));
	
	/* convolution with tri */
	for(i = 0; i < 14000; i++) complete[i] = 0;
	for(i = 0; i < 12000; i++) for(j = 200; j <= 1200; j++) {
		z = i + 200 + j;
		complete[z] += gsl_ran_gamma_pdf(i*0.01,besta,bestb)*0.01 * RNG::triangular_pdf(j*0.01,minincubtime_,maxincubtime_,peakincubtime_)*0.01;
	}
	
	// before normalize
	sum = 0.0;
	for(i=0; i<14000; i++) {
		sum += complete[i];
	}
	printf("Sum before normalization: %lf\n",sum);
	// normalize
	for(i=0; i<14000; i++) complete[i]/=sum;
		
	
	fp = fopen("complete.res","w");
	for(i=0; i<14000; i++) {
		fprintf(fp,"%lf \t %g\n",i*0.01,complete[i]/0.01);
	}
	fclose(fp);
	
#endif	
	
	int day;
	int v,v1,v2;
	double deltareal[40000];
	int deltaD[400];
	int detected[400];
	double cumreal[40000];
	for(i = 0; i < 400; i++) {
		deltaD[i] = 0;
		detected[i] = 0;
	}
	
	/* read deaths */
	fp = fopen("andamento-nazionale.res","r");
		
	if(fgets(riga,1000,fp) == 0) error("Error reading deaths");
	while(fgets(riga,1000,fp) != 0) {
		if(sscanf(riga,"%d %*d %d %*d %d %*d %*d %*d %d",&day,&v1,&v2,&v) == 4) {
			printf("Day %d: %d deltaD, %d new detected\n",day,v,v1+v2);
			deltaD[day] = v;
			detected[day] = v1+v2;
		}
	}		
	if(0 && day < 400) {
		for(day = day+1; day < 400; day++) {
			z = day-174;
			deltaD[day] = (int)(-18.4557 + 5.36411*z -0.226502*z*z + 0.00263803*z*z*z); 
			printf("Fitted death for day %d: %d\n",day,deltaD[day]);
		}	
	}		
	fclose(fp);
	
	// init real
	for(i = 0; i < 40000; i++) deltareal[i] = cumreal[i] = 0.0;
	int checkD = 0;
	// project back
	for(i = 0; i < 400; i++) {
		v = deltaD[i];
		checkD += v;
		j = i*100;
		for(z=0; z<14000; z++) {
			if(j-z >= 0) deltareal[j-z] += v*complete[z] * amplification;
			else deltareal[0] += v*complete[z] * amplification;
		}
	}
	printf("checkD = %d\n",checkD);
	
	// compute and print cumreal
	for(i = 1; i < 40000; i++) cumreal[i] = cumreal[i-1] + deltareal[i-1];
	fp = fopen("cumreal.res","w");
	for(i=0; i<400; i++) if(i >= 129) {
		for(j=1; j<=100; j++) {
			z = (i-129)*100+j;
			fprintf(fp,"%d \t %lf \t %lf\n",z,cumreal[i*100+j],deltareal[i*100+j]);
		}	
	}
	fclose(fp);
	
	// compute and print deltareal (daily)
	fp = fopen("dailyreal.res","w");
	for(i=0; i<400; i++) {
		sum = 0.0;
		for(j=0; j<100; j++) sum += deltareal[i*100+j];
		fprintf(fp,"%d \t %lf\n",i,sum);
	}
	fclose(fp);
	
	// compute detected and print on file
	fp = fopen("detected.res","w");
	for(i=0; i<400; i++) if(129+i < 400) {
		z = detected[129+i];
		for(j=1; j<=100; j++) fprintf(fp,"%d \t %lf\n",i*100+j,z*1.0/100);
	}	
	fclose(fp);
	
	exit(1);
#endif

    
    
}

/*
**  Function : computefactor()
**  Purpose:  compute current factor for mobility restriction countermeasures as function of time  
**              
*/
double Virus::computefactor(double t)
{
    double ret = -1; 
	if(typeoffactor_ == 1) {
		ret = factor_param1_ - factor_param2_ * t;
		if(ret < 0) ret = 0;
	} else if(typeoffactor_ == 2) {
		ret = factor_param1_ + (factor_param2_-factor_param1_)/(factor_param3_-factor_param4_) * (t-anticipate_-factor_param4_);
		if(ret > factor_param1_) ret = factor_param1_;
		if(ret < factor_param2_) ret = factor_param2_;
	} else if(typeoffactor_ == 3) {
		if(t < factor_param4_) ret = factor_param1_ + (factor_param2_-factor_param1_)/factor_param4_ * t;
		else ret = factor_param2_ + (factor_param3_-factor_param2_)/(factor_param5_ - factor_param4_) * (t - factor_param4_);
		if(ret < factor_param6_) ret = factor_param6_;
	} else if(typeoffactor_ == 4) {
		ret = factor_param1_ + (factor_param2_-factor_param1_)/(factor_param3_-factor_param4_) * (t-anticipate_-factor_param4_);
		if(ret > factor_param1_) ret = factor_param1_;
		if(ret < factor_param2_) ret = factor_param2_;
		if(t-anticipate_ > factor_param5_) {
			ret = factor_param7_; 
		}	
	} else {
		error("unknown type of factor");
	}
	if(ret < 0) error("negative factor"); 
	return(ret);	
}

/*
**  Function : computeswabsrate()
**  Purpose:  compute current swabs rate as function of time  
**              
*/
double Virus::computeswabsrate(double t)
{
    double ret = -1; 
	if(typeofswabs_ == 1) {
		ret = swabs_param1_ + swabs_param2_ * t;
		if(ret < 0) ret = 0;
	} else if(typeofswabs_ == 2) {
		if(t < anticipate_) ret = swabs_param1_;
		else ret = swabs_param1_ + swabs_param2_ * (t-anticipate_);
	} else if(typeofswabs_ == 3) {
		if(t < anticipate_) ret = swabs_param1_;
		else ret = swabs_param1_ + swabs_param2_ * (t-anticipate_);
		if(t > swabs_param3_+anticipate_) ret = swabs_param1_ + swabs_param2_ * swabs_param3_;
	} else {
		error("unknown type of swabs");
	}
	if(ret < 0) error("negative swabs rate"); 
	return(ret);	
}

/*
**  Function : sample()
**  Purpose:  discrete time approx  
**              
*/
void Virus::sample()
{
    double now = Scheduler::instance().clock();
	double factor = computefactor(now);
	double swabsrate = computeswabsrate(now);
	double totrate;
	double probk;
	long newinfections;
	long i,j,id,z,deltadetected;
	int bin;
	long currentK, currentR;
	
	if(debug_ > 15) { printf("Time %lf: Virus: called sample\n",now); fflush(stdout); }
	
	// compute total infection rate
	totrate = 0.0;
	totrate += lowrate_ * num_u_incub_;
	totrate += lowrate_ * num_u_pre_symp_ * pasymp_ + highrate_ * num_u_pre_symp_ * (1.0-pasymp_);
	// totrate += highrate_ * num_u_pre_symp_;
	totrate += lowrate_ * num_u_asymp_;
	totrate += highrate_ * num_u_super_;
	totrate += lowrate_ * num_u_residual_;  // are RESIDUALS INFECTIOUS??
	
	// apply factor
	totrate *= factor;
	
	if(debug_ > 15) fprintf(fl_,"%lf \t %lf \t %ld \t %ld\n",now,totrate,num_pos_,cum_pos_);
	
	// optional (time-consuming) computation of R0(t)
	if(computeR0_) {
		
		for(z = 0; z <= num_u_incub_-1; z++) {
			id = incubid_[z]; 
			bin = (int)floor(pos_[id]->arrivaltime_/binR0_);
			if(bin < MAXBINS) sumR0_[bin]+=lowrate_*factor;
		}	
		for(z = 0; z <= num_u_pre_symp_-1; z++) {
			id = presympid_[z]; 
			bin = (int)floor(pos_[id]->arrivaltime_/binR0_);
			if(bin < MAXBINS) sumR0_[bin]+=highrate_*factor;
		}
		for(z = 0; z <= num_u_super_-1; z++) {
			id = superid_[z]; 
			bin = (int)floor(pos_[id]->arrivaltime_/binR0_);
			if(bin < MAXBINS) sumR0_[bin]+=highrate_*factor;
		}
		for(z = 0; z <= num_u_asymp_-1; z++) {
			id = asympid_[z]; 
			bin = (int)floor(pos_[id]->arrivaltime_/binR0_);
			if(bin < MAXBINS) sumR0_[bin]+=lowrate_*factor;
		}
		for(z = 0; z <= num_u_residual_-1; z++) {
			id = residualid_[z]; 
			bin = (int)floor(pos_[id]->arrivaltime_/binR0_);
			if(bin < MAXBINS) sumR0_[bin]+=lowrate_*factor;
		}
		
		
		// computation of detected-based R0(t)
		currentK = num_k_incub_ + num_k_pre_symp_ + num_k_super_ + num_k_symp_ + num_k_residual_ + num_k_asymp_;
		currentR = num_k_dead_ + num_k_recovered_;
		bin = (int)floor(now/binR0_);
		if(bin < MAXBINS) { 
			deltaK_[bin] += (currentK - oldK_);
			sumK_[bin] += currentK * s_->deltasample();
			deltaR_[bin] += (currentR - oldR_);
		}
		oldK_ = currentK;	
		oldR_ = currentR;
	}	
	
	// generate number of new infections
	newinfections = RNG::poisson(totrate * s_->deltasample());
	if(debug_ > 9) printf("Time %lf: Virus: totrate %lf - factor %lf - mupoisson %lf - newinfections %ld\n",
		now, totrate, factor, totrate * s_->deltasample(), newinfections);

#ifdef GIVENREAL
    z = (long)((now+1e-6)/s_->deltasample());
	newinfections = (long)floor(givenreal_[z]);
	if(RNG::uniform()< givenreal_[z]-newinfections) newinfections++;
	reversemu_[z] = givenreal_[z]*factor/(totrate*s_->deltasample());
	if(debug_ > 9) printf("Time %lf: GIVENREAL: givenreal_[%ld] = %lf, newinfections %ld, factor %lf, reversemu[%ld] %lf\n", now, z, givenreal_[z], newinfections, factor, z, reversemu_[z]); 
#endif
		
	for(i = 0; i < newinfections; i++) newinfection();
	
#ifdef GIVENDETECTED
	// compute number of detectable
	long detectable = num_u_incub_ + num_u_pre_symp_ + num_u_asymp_ + num_u_super_ + num_u_residual_;
	z = (long)((now+1e-6)/s_->deltasample());
	if(debug_>9) printf("Discrete time %ld: numdetectable = %ld, givendetected = %lf, determtoq = %ld\n",z,detectable,givendetected_[z],determtoq_[z]);
	double tomove = givendetected_[z] - determtoq_[z];
	// compute rho_[t]
	if(tomove < 0) {
		printf("Warning now = %lf: tomove %lf negative!\n",now,tomove);
		probk = 0;
		rho_[z] = 0;
	} else if(tomove > detectable) {
		printf("Warning now = %lf: tomove %lf > detactable %ld\n",now,tomove,detectable);
		probk = 0;
		rho_[z] = 0;
	} else {	
		rho_[z] = -1.0/s_->deltasample()*log(1.0-tomove/detectable);
		if(debug_>9) printf("Discrete time %ld: rho = %lf\n",z,rho_[z]);
		probk = tomove/detectable;
	}	
#else 	
	// swabs process
	probk = 1.0-exp(-swabsrate*s_->deltasample());
	if(debug_ > 9) printf("Time %lf: Virus: factor %lf, swabsrate %lf - probk %lf\n",now-anticipate_, factor, swabsrate, probk);
#endif	
	
	deltadetected = 0;
	// comment out if incubation is not detected !
	i = RNG::binom(probk, num_u_incub_);
	deltadetected += i;
	if(debug_ > 20) printf("delta_incub %ld\n",i);
	for(j = 0; j < i; j++) {
		z = RNG::uniformint(0,num_u_incub_-1);
		id = incubid_[z];
		num_k_incub_++;
		incubid_[z] = incubid_[num_u_incub_-1];
		pos_[incubid_[z]]->revindex_ = z;
		num_u_incub_--;
		if(pos_[id]->state_ != U_INCUB) { printf("extracted id %ld in state %s\n",id,Pos::statestring(pos_[id]->state_)); error("wrong state"); }
		pos_[id]->state_ = K_INCUB;
		if(debug_ > 10) printf("Time %lf: Virus: swabs to id %ld changes its state to %s\n",now, id, Pos::statestring(pos_[id]->state_));
	}
	
	i = RNG::binom(probk, num_u_pre_symp_);
	deltadetected += i;
	if(debug_ > 20) printf("delta_presymp %ld\n",i);
	for(j = 0; j < i; j++) {
		z = RNG::uniformint(0,num_u_pre_symp_-1);
		id = presympid_[z];
		num_k_pre_symp_++;
		presympid_[z] = presympid_[num_u_pre_symp_-1];
		pos_[presympid_[z]]->revindex_ = z;
		num_u_pre_symp_--;
		if(pos_[id]->state_ != U_PRE_SYMP) error("wrong state");
		pos_[id]->state_ = K_PRE_SYMP;
		if(debug_ > 10) printf("Time %lf: Virus: swabs to id %ld changes its state to %s\n",now, id, Pos::statestring(pos_[id]->state_));
	}
	
	i = RNG::binom(probk, num_u_asymp_);
	deltadetected += i;
	if(debug_ > 20) printf("delta_asymp %ld\n",i);
	for(j = 0; j < i; j++) {
		z = RNG::uniformint(0,num_u_asymp_-1);
		id = asympid_[z];
		num_k_asymp_++;
		asympid_[z] = asympid_[num_u_asymp_-1];
		pos_[asympid_[z]]->revindex_ = z;
		num_u_asymp_--;
		if(pos_[id]->state_ != U_ASYMP) error("wrong state");
		pos_[id]->state_ = K_ASYMP;
		if(debug_ > 10) printf("Time %lf: Virus: swabs to id %ld changes its state to %s\n",now, id, Pos::statestring(pos_[id]->state_));
	}
	
	i = RNG::binom(probk, num_u_super_);
	deltadetected += i;
	if(debug_ > 20) printf("delta_super %ld\n",i);
	for(j = 0; j < i; j++) {
		z = RNG::uniformint(0,num_u_super_-1);
		id = superid_[z];
		num_k_super_++;
		superid_[z] = superid_[num_u_super_-1];
		pos_[superid_[z]]->revindex_ = z;
		num_u_super_--;
		if(pos_[id]->state_ != U_SUPER) error("wrong state");
		pos_[id]->state_ = K_SUPER;
		if(debug_ > 10) printf("Time %lf: Virus: swabs to id %ld changes its state to %s\n",now, id, Pos::statestring(pos_[id]->state_));
	}
	
	i = RNG::binom(probk, num_u_residual_);
	if(debug_ > 20) printf("delta_residual %ld\n",i);
	deltadetected += i;
	for(j = 0; j < i; j++) {
		z = RNG::uniformint(0,num_u_residual_-1);
		id = residualid_[z];
		num_k_residual_++;
		residualid_[z] = residualid_[num_u_residual_-1];
		pos_[residualid_[z]]->revindex_ = z;
		num_u_residual_--;
		if(pos_[id]->state_ != U_RESIDUAL) error("wrong state");
		pos_[id]->state_ = K_RESIDUAL;
		if(debug_ > 10) printf("Time %lf: Virus: swabs to id %ld changes its state to %s\n",now, id, Pos::statestring(pos_[id]->state_));
	}	
	
	if(debug_>9) printf("Time %lf: effective moved = %ld\n",now,deltadetected);
}

/*
**  Function : newinfection()
**  Purpose:  birth of a new positive  
**              
*/
void Virus::newinfection()
{
	double now = Scheduler::instance().clock();
	long id;
	Pos* pos; 
	PosEvent* pe;
	double delta;

	if(numfreeid_ <= 0) error("Exhausted ids");

	// get an id
	id = freeid_[numfreeid_--];
	if(debug_ > 15) printf("newinfection: chosen index %ld\n",id); 
		
	// alloc a Pos
	pos = Pos::alloc(id);
		
	pos->arrivaltime_ = now;
	pos->incubtime_ = genincub();
	
	if(pos->incubtime_ > 0*presicktime_) {
	
		pos->state_ = U_INCUB;
		pos->revindex_ = num_u_incub_;
		incubid_[num_u_incub_++] = id;
		pe = PosEvent::alloc(STARTPRESYMP,id);
		// delta = pos->incubtime_ - presicktime_;
		delta = pos->incubtime_;
		Scheduler::instance().schedule(this, pe, delta);
		
	} else {
		
		pos->state_ = U_PRE_SYMP;
		pos->revindex_ = num_u_pre_symp_;
		presympid_[num_u_pre_symp_++] = id;
		pe = PosEvent::alloc(ENDPRESYMP,id);
		delta = pos->incubtime_;
		Scheduler::instance().schedule(this, pe, delta);
	}
	
	// default flags
	todeath_[id] = intensive_[id] = 0;
	 
	// save in array
	if(pos_[id]) { printf("Found pos in pos_[%ld] with id %ld in state %s\n",id,pos_[id]->id_,Pos::statestring(pos_[id]->state_)); error("already a pos here"); }
	pos_[id] = pos; 
	num_pos_++;
	cum_pos_++;
	
	if(debug_>15) fprintf(stdout,"# %f : Virus : newinfection %ld with incubtime %lf, now in state %s, revindex %ld \n", now, id, pos->incubtime_, Pos::statestring(pos->state_), pos->revindex_);
}

/*
**  Function : handle_startpresymp(long id);
**  Purpose:  transition INCUB -> PRESYMP of positive id  
**              
*/
void Virus::handle_startpresymp(long id)
{
	double now = Scheduler::instance().clock();
	Pos* pos; 
	PosEvent* pe;
	double delta;
	long z = -1;

	if(debug_>15) { fprintf(stdout,"# %f : Virus : called start presymp of %ld \n", now, id); fflush(stdout); }

	// get pos
	pos = pos_[id]; 
	
	if(pos->state_ != U_INCUB && pos->state_ != K_INCUB) error("wrong state"); 
	
	if(pos->state_ == U_INCUB) {
		num_u_incub_--;
		z = pos->revindex_;
		incubid_[z] = incubid_[num_u_incub_];
		pos_[incubid_[z]]->revindex_ = z;
		pos->state_ = U_PRE_SYMP;
		pos->revindex_ = num_u_pre_symp_;
		presympid_[num_u_pre_symp_++] = id;
	} else {
		num_k_incub_--;
		pos->state_ = K_PRE_SYMP;
		num_k_pre_symp_++;
	}	
	
	pe = PosEvent::alloc(ENDPRESYMP,id);
	delta = presicktime_;
	Scheduler::instance().schedule(this, pe, delta);

	if(debug_>10) fprintf(stdout,"# %f : Virus : transition INCUB->PRESYMP of %ld, old revindex %ld, new revindex %ld\n", now, id, z, pos->revindex_);	
}

/*
**  Function : handle_endpresymp(long id);
**  Purpose:  transition PRESYMP -> <various> of positive id  
**              
*/
void Virus::handle_endpresymp(long id)
{
	double now = Scheduler::instance().clock();
	Pos* pos; 
	PosEvent* pe;
	double u = RNG::uniform();
	PosState newstate, oldstate;
	double delta, v;
	long z = -1;
	
	if(debug_>15) { fprintf(stdout,"# %f : Virus : called end presymp of %ld \n", now, id); fflush(stdout); }

	// get pos
	pos = pos_[id]; 
	oldstate = pos->state_;
	
	if(oldstate != U_PRE_SYMP && oldstate != K_PRE_SYMP) error("wrong state"); 
	delta = gensick();
	
	if(oldstate == U_PRE_SYMP) {
		num_u_pre_symp_--;
		z = pos->revindex_;
		presympid_[z] = presympid_[num_u_pre_symp_];
		pos_[presympid_[z]]->revindex_ = z;
				
		if(u < pasymp_) {
			pos->revindex_ = num_u_asymp_;
			asympid_[num_u_asymp_++] = id;
			pos->state_ = newstate = U_ASYMP;
		} else if(u < pasymp_ + psuper_) {
			pos->revindex_ = num_u_super_;
			superid_[num_u_super_++] = id;
			pos->state_ = newstate = U_SUPER;
		} else {	
			// infinite swabs rate for symp
			num_k_symp_++;
			pos->state_ = newstate = K_SYMP;
			determtoq_[(int)ceil(now/s_->deltasample())]++;
		}	
	} else {
		num_k_pre_symp_--;
		if(u < pasymp_) {
			num_k_asymp_++;
			pos->state_ = newstate = K_ASYMP;
		} else if(u < pasymp_ + psuper_) {
			num_k_super_++;
			pos->state_ = newstate = K_SUPER;
		} else {	
			num_k_symp_++;
			pos->state_ = newstate = K_SYMP;
		}	 
	}	
	
	pe = PosEvent::alloc(ENDSICK,id);
	Scheduler::instance().schedule(this, pe, delta);
	if(debug_>10) fprintf(stdout,"# %f : Virus : transition of %ld: %s -> %s, old revindex %ld, new revindex %ld\n", now, id, Pos::statestring(oldstate), Pos::statestring(newstate),z,pos->revindex_);
}


/*
**  Function : handle_endsick(long id);
**  Purpose:  transitions after sick time  
**              
*/
void Virus::handle_endsick(long id)
{
	double now = Scheduler::instance().clock();
	Pos* pos; 
	PosEvent* pe;
	double u = RNG::uniform();
	PosState newstate, oldstate;
	double delta, v;
	long z = -1;
	
	if(debug_>15) { fprintf(stdout,"# %f : Virus : called end sick of %ld \n", now, id); fflush(stdout); }

	// get pos
	pos = pos_[id]; 
	oldstate = newstate = pos->state_;
	
	if(oldstate != U_ASYMP && oldstate != K_ASYMP && oldstate != U_SUPER && oldstate != K_SUPER && oldstate != K_SYMP) error("wrong state"); 
	
	switch (oldstate) {
	
		case U_SUPER:
		
			num_u_super_--;
			z = pos->revindex_;
			superid_[z] = superid_[num_u_super_];
			pos_[superid_[z]]->revindex_ = z;
			
			pos->state_ = newstate = U_RESIDUAL;
			pos->revindex_ = num_u_residual_;
			residualid_[num_u_residual_++] = id;
			break;
		
		case K_SUPER:
		
			num_k_super_--;
			num_k_residual_++;
			pos->state_ = newstate = K_RESIDUAL;
			break;
		
		case U_ASYMP:
		
			num_u_asymp_--;
			z = pos->revindex_;
			asympid_[z] = asympid_[num_u_asymp_];
			pos_[asympid_[z]]->revindex_ = z;
	
			pos->state_ = newstate = U_RESIDUAL;
			pos->revindex_ = num_u_residual_;
			residualid_[num_u_residual_++] = id;
			break;
		
		case K_ASYMP:
		
			num_k_asymp_--;
			num_k_residual_++;
			pos->state_ = newstate = K_RESIDUAL;
			break;	
			
		case K_SYMP:
	
			num_k_symp_--;
			v = RNG::uniform();
			if(v < pdeath_ + pintensive_) {
				num_k_intensive_++;
				intensive_[id] = 1;
				if(v < pdeath_) todeath_[id] = 1; 
				else todeath_[id] = 0;
				pos->state_ = newstate = K_INTENSIVE; 
			} else {
				num_k_residual_++;	
				intensive_[id] = 0;
				todeath_[id] = 0;
				pos->state_ = newstate = K_RESIDUAL; 
			}	
			break;
			
		default:
		
			error("wrong state");		
	}		
			
	if(intensive_[id]) {
			if(pos->state_ != K_INTENSIVE) error("Hey, this should not happen");
			pe = PosEvent::alloc(ENDINTE,id);
			delta = gendeath();
			if(todeath_[id]) delta/=1;
	} else {
			if(pos->state_ != K_RESIDUAL && pos->state_ != U_RESIDUAL) error("Hoy, this should not happen");
			delta = genresidual();
			pe = PosEvent::alloc(ENDRESIDUAL,id);
	}		
			
	Scheduler::instance().schedule(this, pe, delta);
	if(debug_>10) fprintf(stdout,"# %f : Virus : transition of %ld: %s -> %s, old revindex %ld, new revindex %ld\n", now, id, Pos::statestring(oldstate), Pos::statestring(newstate),z,pos->revindex_);
}



/*
**  Function : handle_endinte(long id);
**  Purpose:  removal of positive id in intensive therapy 
**              
*/
void Virus::handle_endinte(long id)
{
	double now = Scheduler::instance().clock();
	Pos* pos; 
	PosEvent* pe;
	long z = -1;
	double delta = -1;
	PosState newstate, oldstate;
	
	// get pos
	pos = pos_[id]; 
	oldstate = newstate = pos->state_;
	if(debug_>15) { fprintf(stdout,"# %f : Virus : called endinte of %ld now in state %s\n", now, id, Pos::statestring(oldstate)); fflush(stdout); }
	
	switch(pos->state_) {
		
		case K_INTENSIVE: 
			
			if(todeath_[id]) delta = 1;
			else delta = genresidual();
			num_k_intensive_--;
			num_k_residual_++;
			pos->state_ = newstate = K_RESIDUAL; 
			break;
			
		default: 
		
			error("wrong state");		
			
	}
	
	pe = PosEvent::alloc(ENDRESIDUAL,id);
	Scheduler::instance().schedule(this, pe, delta);
	if(debug_>10) fprintf(stdout,"# %f : Virus : transition of %ld: %s -> %s, old revindex %ld, new revindex %ld\n", now, id, Pos::statestring(oldstate), Pos::statestring(newstate),z,pos->revindex_);
}


/*
**  Function : handle_endresidual(long id);
**  Purpose:  removal of positive id  
**              
*/
void Virus::handle_endresidual(long id)
{
	double now = Scheduler::instance().clock();
	Pos* pos; 
	PosEvent* pe;
	long z = -1;
	PosState oldstate;

	// get pos
	pos = pos_[id]; 
	oldstate = pos->state_;
	
	if(debug_>15) { fprintf(stdout,"# %f : Virus : called endresidual of %ld in state %s\n", now, id, Pos::statestring(oldstate)); fflush(stdout); }
	
	switch(pos->state_) {
		
		case K_RESIDUAL: 
			
			num_k_residual_--;
			if(todeath_[id]) num_k_dead_++; else num_k_recovered_++;
			break;
		
		case U_RESIDUAL:
		
			num_u_residual_--;
			z = pos->revindex_;
			residualid_[z] = residualid_[num_u_residual_];
			pos_[residualid_[z]]->revindex_ = z;
			
			if(todeath_[id]) num_u_dead_++; else num_u_recovered_++;
			break;
			
		default: 
		
			error("wrong state");		
			
	}
	
	num_pos_--;
	
	// computation of R0
	if(computeR0_) {
		int bin = (int)floor(pos->arrivaltime_/binR0_);
		if(bin < MAXBINS) numR0_[bin]++;
	}	
		
	pos_[id] = 0;
	Pos::free(pos);
	
	// free id
	numfreeid_++;
	freeid_[numfreeid_] = id;
	
	if(debug_>10) fprintf(stdout,"# %f : Virus : removal of %ld, old revindex %ld\n", now, id, z);
}


/*
**  Function : init()
**  Purpose:  initializations at the beginning of simulation  
**              
*/
void Virus::init()
{
	double now = Scheduler::instance().clock();
	long i,id;
	Pos* pos; 
	
	num_u_incub_ = num_k_incub_ = num_u_pre_symp_ = num_k_pre_symp_ = num_u_asymp_ = num_k_asymp_ = num_pos_ = cum_pos_ = num_k_intensive_ = 0;
	num_u_super_ = num_k_super_ = num_u_symp_ = num_k_symp_ = num_u_recovered_ = num_k_recovered_ = num_u_dead_ = num_k_dead_ = 0;
	num_u_residual_ = num_k_residual_ = num_u_intensive_ = num_k_intensive_ = 0;
	
	old_num_u_incub_ = old_num_k_incub_ = old_num_u_pre_symp_ = old_num_k_pre_symp_ = old_num_u_asymp_ = old_num_k_asymp_ = old_num_u_super_ = old_num_k_intensive_ = 0;
	old_num_k_super_ = old_num_u_symp_ = old_num_k_symp_ = old_num_u_recovered_ = old_num_k_recovered_ = old_num_u_dead_ = old_num_k_dead_ = 0;
	old_num_u_residual_ = old_num_k_residual_ = old_num_u_intensive_ = old_num_k_intensive_;
	
	for(i = 1; i <= MAXPOS; i++) freeid_[i] = MAXPOS-i+1;
	numfreeid_ = MAXPOS;
	
	if(computeR0_) {
		for(i = 0; i < MAXBINS; i++) { sumR0_[i] = 0.0; numR0_[i] = 0; deltaK_[i] = 0.0; sumK_[i] = 0.0; deltaR_[i] = 0; }
		oldK_ = oldR_ = 0;
	}	
	
	for(i = 0; i < initialpos_; i++) newinfection();
	
	printsituation();
	
	if(debug_ > 1) fprintf(stdout,"# %f : Virus: done init \n", now);
}

/*
**  Function : reset()
**  Purpose:  reset counters  
**              
*/
void Virus::reset()
{
	double now = Scheduler::instance().clock();
	if(debug_>1) fprintf(fl_,"# %f : Virus reset\n", now);
}


/*
**  Function : finish()
**  Purpose:  release memory  
**              
*/
void Virus::finish()
{
        long i;
        double now = Scheduler::instance().clock();
        double estim; 
        FILE* fo = fopen("Rt.res","w");
        
        if(debug_>1) fprintf(fl_,"# %f : Virus : called finish\n", now);
        for(i = 1; i <= MAXPOS; i++) if(pos_[i]) Pos::free(pos_[i]);
        
        // computation of R0
        if(debug_>1) printf("\n### Results for R0(t)\n");
        fprintf(fr_,"\n### Results for R0(t)\n");
        int maxbin = (int)floor((now-guard_)/binR0_);
        for(i=0; i < maxbin && i < MAXBINS; i++) if(numR0_[i] > 0) {
			sumR0_[i] *= s_->deltasample();
			if(deltaR_[i] > 0) estim = deltaK_[i]/deltaR_[i]; else estim = 0.0; 
			if(debug_>1) printf("R0(%lf) = %lf (estimated from detected: %lf with deltaK %lf, deltaR %lf)\n",i*binR0_,sumR0_[i]/numR0_[i],estim+1,deltaK_[i],deltaR_[i]);
			fprintf(fo,"%lf \t %lf \t %lf \t %ld \t %lf \t %lf \t %lf\n",i*binR0_-anticipate_,sumR0_[i]/numR0_[i],sumR0_[i],numR0_[i],estim+1,deltaK_[i],deltaR_[i]);
		}	
		fclose(fo);
		
		
		fo = fopen("reverse.res","w");
		for(i = 0; i<=now*100; i++) if(i >= 3000) {
				fprintf(fo,"%lf \t %lf \t %lf\n",i*0.01,rho_[i],reversemu_[i]);
		}
		fclose(fo);		
}

/*
**  Function : handle()
**  Purpose:   handle all events  
**              
*/	     
void Virus::handle(Event *e)
{
	PosEvent* pe = (PosEvent*)e;			/* must be a pos event */
	double now = Scheduler::instance().clock();
	
	if(debug_>15) fprintf(fl_,"# %f : Virus : handle event %d\n", now, pe->type());
	
	switch(pe->type()) 
	{
		case NEWINFECTION :
		
		        newinfection();	
				break;									
			
		case STARTPRESYMP :
		
		        handle_startpresymp(pe->id());	
				break;										
			
		case ENDPRESYMP : 
		
		        handle_endpresymp(pe->id());	
		        break;
		        
		case ENDSICK : 
		
		        handle_endsick(pe->id());	
		        break;	
		        
		case ENDINTE : 
		
		        handle_endinte(pe->id());	
		        break;	        
		        
		case ENDRESIDUAL : 
		
		        handle_endresidual(pe->id());	
		        break;	        	        
			
		default: error("Unrecognized Pos Eevent");
	}
	
	PosEvent::free(pe);						
}	


/*
**  Function : printsituation()
**  Purpose:  print current statistics  
**              
*/
void Virus::printsituation()
{
    double now = Scheduler::instance().clock();
    long tot,z,id;
    long d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,d16,d17,d18;
    long totk, totu, deltak, deltau, attualu;
	
	tot = 0;
	if(debug_ > 2) {
		tot = num_u_incub_ + num_k_incub_ + num_u_pre_symp_ + num_k_pre_symp_ + num_u_asymp_ + 
		      num_k_asymp_ + num_u_super_ + num_k_super_ + num_u_symp_ + num_k_symp_ + num_u_residual_ + num_k_residual_ + num_k_intensive_ + num_u_intensive_;  
		if(tot != num_pos_) { printf("tot = %ld - num_pos = %ld - cum_pos = %ld\n",tot,num_pos_,cum_pos_); error("sanity check 1 failed"); }
		tot += num_u_dead_ + num_k_dead_ + num_u_recovered_ + num_k_recovered_;
		if(tot != cum_pos_) error("sanity check 2 failed");
	}	
	
	d1 = -old_num_u_incub_ + num_u_incub_;
	d2 = -old_num_k_incub_ + num_k_incub_;
	d3 = -old_num_u_pre_symp_ + num_u_pre_symp_;
	d4 = -old_num_k_pre_symp_ + num_k_pre_symp_;
	d5 = -old_num_u_asymp_ + num_u_asymp_;
	d6 = -old_num_k_asymp_ + num_k_asymp_;
	d7 = -old_num_u_super_ + num_u_super_;
	d8 = -old_num_k_super_ + num_k_super_;
	d9 = -old_num_u_symp_ + num_u_symp_;
	d10 = -old_num_k_symp_ + num_k_symp_;
	d11 = -old_num_u_dead_ + num_u_dead_;
	d12 = -old_num_k_dead_ + num_k_dead_;
	d13 = -old_num_u_recovered_ + num_u_recovered_;
	d14 = -old_num_k_recovered_ + num_k_recovered_;
	d15 = -old_num_u_residual_ + num_u_residual_;
	d16 = -old_num_k_residual_ + num_k_residual_;
	d17 = -old_num_u_intensive_ + num_u_intensive_;
	d18 = -old_num_k_intensive_ + num_k_intensive_;
			
	printf("Time %3.0lf: incub [%ld,%ld]-presymp [%ld,%ld]-asymp [%ld,%ld]-super [%ld,%ld]-symp [%ld,%ld]-dead [%ld,%ld]-recov [%ld,%ld]-resid [%ld,%ld]-inte[%ld,%ld]\n",
		now-anticipate_, num_u_incub_, num_k_incub_, num_u_pre_symp_, num_k_pre_symp_, num_u_asymp_, num_k_asymp_, num_u_super_, num_k_super_, num_u_symp_, num_k_symp_, num_u_dead_, 
		num_k_dead_, num_u_recovered_, num_k_recovered_, num_u_residual_, num_k_residual_, num_u_intensive_, num_k_intensive_);
	fprintf(ft_,"%lf \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld "
		"\t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld\n", 
		now-anticipate_, num_u_incub_, d1, num_k_incub_, d2, num_u_pre_symp_, d3, num_k_pre_symp_, d4, num_u_asymp_, d5, num_k_asymp_, d6, num_u_super_, d7, num_k_super_, d8, num_u_symp_, d9, num_k_symp_, d10, 
		num_u_dead_, d11, num_k_dead_, d12, num_u_recovered_, d13, num_k_recovered_, d14, num_u_residual_, d15, num_k_residual_, d16, num_u_intensive_, d17, num_k_intensive_, d18);
				
	/* simplified results */
	totk = num_k_incub_ + num_k_pre_symp_ + num_k_asymp_ + num_k_super_ + num_k_symp_ + num_k_residual_ + num_k_intensive_ +  num_k_dead_ + num_k_recovered_;
	totu = num_u_incub_ + num_u_pre_symp_ + num_u_asymp_ + num_u_super_ + num_u_symp_ + num_u_residual_ + num_u_intensive_ +  num_u_dead_ + num_u_recovered_;
	deltak = d2 + d4 + d6 + d8 + d10 + d12 + d14 + d16 + d18;
	deltau = d1 + d3 + d5 + d7 + d9 + d11 + d13 + d15 + d17;
	attualu = num_u_incub_ + num_u_pre_symp_ + num_u_asymp_ + num_u_super_ + num_u_symp_ + num_u_residual_;
	if(debug_ > 2 && tot != totk + totu) error("sanity check 3 failed");
	z = (long)(now/s_->deltasample())-1;
	// printf("now = %lf - z = %ld\n",now,z);
	fprintf(fr_,"%lf \t %ld \t %ld \t %ld \t %ld \t %ld \t %ld \t %lf \t %ld \t %lf\n",now-anticipate_,totk+totu,deltak+deltau,totk,deltak,totu,deltau,rho_[z],attualu,reversemu_[z]);
				
	old_num_u_incub_ = num_u_incub_;
	old_num_k_incub_ = num_k_incub_;
	old_num_u_pre_symp_ = num_u_pre_symp_;
	old_num_k_pre_symp_ = num_k_pre_symp_;
	old_num_u_asymp_ = num_u_asymp_;
	old_num_k_asymp_ = num_k_asymp_;
	old_num_u_super_ = num_u_super_;
	old_num_k_super_ = num_k_super_;
	old_num_u_symp_ = num_u_symp_;
	old_num_k_symp_ = num_k_symp_;
	old_num_u_dead_ = num_u_dead_;
	old_num_k_dead_ = num_k_dead_;
	old_num_u_recovered_ = num_u_recovered_;
	old_num_k_recovered_ = num_k_recovered_;
	old_num_u_residual_ = num_u_residual_;
	old_num_k_residual_ = num_k_residual_;
	old_num_u_intensive_ = num_u_intensive_;	
	old_num_k_intensive_ = num_k_intensive_;	
		
	if(debug_ > 15) { 
		printf("state of vector freeid: ["); for(z = 10; z > 0; z--) printf("%ld,",freeid_[numfreeid_-z]); printf("%ld]\n",freeid_[numfreeid_]); 
		printf("state of vector incubid: ["); for(z = 0; z < num_u_incub_-1; z++) printf("%ld,",incubid_[z]); if(num_u_incub_ > 0) printf("%ld]\n",incubid_[num_u_incub_-1]); else printf("]\n");
		printf("state of vector presympid: ["); for(z = 0; z < num_u_pre_symp_-1; z++) printf("%ld,",presympid_[z]); if(num_u_pre_symp_ > 0) printf("%ld]\n",presympid_[num_u_pre_symp_-1]); else printf("]\n");
		printf("state of vector superid: ["); for(z = 0; z < num_u_super_-1; z++) printf("%ld,",superid_[z]); if(num_u_super_ > 0) printf("%ld]\n",superid_[num_u_super_-1]); else printf("]\n");
		printf("state of vector asympid: ["); for(z = 0; z < num_u_asymp_-1; z++) printf("%ld,",asympid_[z]); if(num_u_asymp_ > 0) printf("%ld]\n",asympid_[num_u_asymp_-1]); else printf("]\n"); 	
		printf("state of vector residualid: ["); for(z = 0; z < num_u_residual_-1; z++) printf("%ld,",residualid_[z]); if(num_u_residual_ > 0) printf("%ld]\n",residualid_[num_u_residual_-1]); else printf("]\n");
	}	
	
	// sanity check of reverse index
	if(debug_ > 10) {
		for(z = 0; z <= num_u_incub_-1; z++) {
			id = incubid_[z]; if(pos_[id]->revindex_ != z) { printf("Sanity check failed for uncubid %ld: wrong reverse index %ld, should be %ld\n",id,pos_[id]->revindex_,z); error("stop"); }
		}
		for(z = 0; z <= num_u_pre_symp_-1; z++) {
			id = presympid_[z]; if(pos_[id]->revindex_ != z) { printf("Sanity check failed for presympid %ld: wrong reverse index %ld, should be %ld\n",id,pos_[id]->revindex_,z); error("stop"); }
		}
		for(z = 0; z <= num_u_super_-1; z++) {
			id = superid_[z]; if(pos_[id]->revindex_ != z) { printf("Sanity check failed for superid %ld: wrong reverse index %ld, should be %ld\n",id,pos_[id]->revindex_,z); error("stop"); }
		}
		for(z = 0; z <= num_u_asymp_-1; z++) {
			id = asympid_[z]; if(pos_[id]->revindex_ != z) { printf("Sanity check failed for asympid %ld: wrong reverse index %ld, should be %ld\n",id,pos_[id]->revindex_,z); error("stop"); }
		}
		for(z = 0; z <= num_u_residual_-1; z++) {
			id = residualid_[z]; if(pos_[id]->revindex_ != z) { printf("Sanity check failed for residualid %ld: wrong reverse index %ld, should be %ld\n",id,pos_[id]->revindex_,z); error("stop"); }
		}
	}			
}

/*
**  Function : test_tri()
**  Purpose:  test triangular distribution generator 
**              
*/
void Virus::test_tri()
{
        long i, bin;
        double r;
        double pdf_[100];
        FILE* fo = fopen("test.tri","w");
        // printf("Test tri with a = %lf, b = %lf, c = %lf\n",minsicktime_,maxsicktime_,peaksicktime_);
        printf("Test tri with a = %lf, b = %lf, c = %lf\n",minincubtime_,maxincubtime_,peakincubtime_);
        for(i = 0; i < 100; i++) pdf_[i] = 0; 
        for(i = 0; i < 10000; i++) {
			r = RNG::triangular(minincubtime_,maxincubtime_,peakincubtime_);			
			// r = RNG::triangular(minsicktime_,maxsicktime_,peaksicktime_); 
			bin = (int)floor(r);
			pdf_[bin]++;
		}
		for(i = 0; i < 100; i++) if(pdf_[i] > 0) {
			 fprintf(fo,"%ld \t %lf\n",i,pdf_[i]*1.0/10000);
		}	
		fclose(fo);
}

