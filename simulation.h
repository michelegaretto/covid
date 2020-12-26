/*
**  File:        simulation.h
**  Program:     corona
**  Purpose:     class Simulation      
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#ifndef simulation_h
#define simulation_h

#include "service.h"
#include "scheduler.h"
#include "timer-handler.h"
#include "virus.h"

#define MAXAGGR 10

class Simulation;

class TranTimer : public TimerHandler {
public:

	TranTimer(Simulation *s) : s_(s) {}
 	virtual void expire(Event *e);
protected:
	Simulation * s_;
};

class DurationTimer : public TimerHandler {
public:

	DurationTimer(Simulation *s) { s_ = s; }
 	virtual void expire(Event *e);
protected:
 	Simulation * s_;
};

class BatchTimer : public TimerHandler {
public:

	BatchTimer(Simulation *s) { s_ = s; }
 	virtual void expire(Event *e);
protected:
 	Simulation * s_;
};

class PrintTimer : public TimerHandler {
public:

	PrintTimer(Simulation *s) { s_ = s; }
 	virtual void expire(Event *e);
protected:
 	Simulation * s_;
};

class SampleTimer : public TimerHandler {
public:

	SampleTimer(Simulation *s) { s_ = s; }
 	virtual void expire(Event *e);
protected:
 	Simulation * s_;
};

class Simulation : public Handler {
friend class Process;
public:

	Simulation() : numbatches_(NUMBATCHES), fl_(stdout) {}
	
	static Simulation& instance() { return (*instance_); }
	
	inline double& transient() { return transient_; }
	inline int numbatches() { return numbatches_; }
	inline int debug() { return debug_; }
	inline int runsim() { return runsim_; }
	inline double& durbatch() { return durbatch_; }
	inline double& duration() { return duration_; }
	inline double& deltasample() { return delta_sample_; }
	inline void setvirus(Virus*v) { v_ = v; }
	
	void handle(Event *);
	void sample();
	void inputparameters();
	void reset();
	void print();
	void startbatch();
	void batch();
	void analyzebatches() { }
	void startsim();
	void printresults();
	void finish();
		
protected:

    /* SIMULATION PARAMETERS */
	int runsim_; 		  /* flag */
	double transient_;    /* transient duration (s) */
	double duration_;	  /* simulation duration */
	double delta_sample_; /* sample interval (uniform) */
	double delta_print_;  /* print interval (uniform) */
	
	int numbatches_;
	int batch_;
	double durbatch_;	 /* batch duration (s) */

	BatchTimer* bt_;
	DurationTimer* dt_;
	PrintTimer* pt_;     /* timer for stats print */
	TranTimer* rt_;      /* timer for transient */
	SampleTimer* st_;	 /* timer for approx sampling */
		
	/* STATISTICS COLLECTION */
    unsigned long numsamples_; 
    double batchmeans_[MAXAGGR+1][NUMBATCHES+1];
	
	/* MISCELLANEOUS VARIABLES */
	char name_[MAXCAR];	 /* simulation name (used to print out results) */
	int debug_;		 /* debug level */
	FILE* fr_;		 /* output file for results */ 
	FILE* ft_;		 /* output file for trace */ 
	FILE* fl_;		 /* output file for log */
	
private:

	Virus* v_;		        /* associated virus object */
	static Simulation* instance_;			
};

#endif

	
