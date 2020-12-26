/*
**  File:        timer_handler.cc
**  Program:     VOD
**  Purpose:     class TimerHandler    
**  Author:      Michele Garetto
**  Last Update: November 5 2010 
*/

#include <stdlib.h>  // abort()
#include <stdio.h>

#include "timer-handler.h"

void
TimerHandler::cancel()
{
	if (status_ != TIMER_PENDING) {
		fprintf(stderr,
		  "Attempting to cancel timer at %p which is not scheduled\n",
		  this);
		abort();
	}
	_cancel();
	status_ = TIMER_IDLE;
}

/* sched checks the state of the timer before shceduling the
 * event. It the timer is already set, abort is called.
 * This is different than the OTcl timers in tcl/ex/timer.tcl,
 * where sched is the same as reshced, and no timer state is kept.
 */
void
TimerHandler::sched(double delay)
{
	if (status_ != TIMER_IDLE) {
		fprintf(stderr,"Couldn't schedule timer");
		abort();
	}
	_sched(delay);
	status_ = TIMER_PENDING;
}

void
TimerHandler::resched(double delay)
{
	if (status_ == TIMER_PENDING)
		_cancel();
	_sched(delay);
	status_ = TIMER_PENDING;
}

void
TimerHandler::handle(Event *e)
{
	if (status_ != TIMER_PENDING)   // sanity check
		abort();
	status_ = TIMER_HANDLING;
	expire(e);
	// if it wasn't rescheduled, it's done
	if (status_ == TIMER_HANDLING)
		status_ = TIMER_IDLE;
}
