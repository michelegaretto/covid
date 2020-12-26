/*
**  File:        main.cc
**  Program:     corona 
**  Purpose:     Simulation of coronavirus 
**  Version:     1.0
**               - basic simulation engine
**  Author:      Michele Garetto
**  Last Update: 2020
**  Requires:    gsl library (gnu scientific library)
*/

#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "service.h"
#include "simulation.h"
#include "pos.h"
#include "posevent.h"


/* definition of the Random Number Generator */
gsl_rng* RNG::rng_ = gsl_rng_alloc(gsl_rng_mt19937);	 /* MT19937 Matsumoto, Nishimura */
unsigned long int RNG::seed_ = 0;
Pos* Pos::free_ = 0;
const char Pos::StateString[18][20] = { "U_INCUB", "K_INCUB", "U_PRE_SYMP", "K_PRE_SYMP", "U_SYMP", "K_SYMP", 
	"U_SUPER", "K_SUPER", "U_ASYMP", "K_ASYMP", "U_RECOVERED", "K_RECOVERED", "U_DEAD", "K_DEAD", "U_RESIDUAL", "K_RESIDUAL", "U_INTENSIVE", "K_INTENSIVE" };	
PosEvent* PosEvent::free_ = 0;

// Scheduler* Scheduler::instance_ = new ListScheduler();
Scheduler* Scheduler::instance_ = new HeapScheduler();
Simulation* Simulation::instance_ = new Simulation();

long Scheduler::uid_ = 1;

int main(int argc, char *argv[]) {
        
        RNG::inputseed();
	Simulation* sim = &Simulation::instance();
        sim->inputparameters();
	
	Virus* virus = new Virus(sim); 
	virus->inputparameters();
	sim->setvirus(virus);
	
	//vvirus->test_tri();
		
	if(sim->runsim()) {       	
	        sim->startsim();
	        Scheduler::instance().run();	
	}
		
	return 0;
}
