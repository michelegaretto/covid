/*
**  File:        timer_handler.h
**  Program:     VOD
**  Purpose:     class TimerHandler    
**  Author:      Michele Garetto
**  Last Update: November 5 2010 
*/

#ifndef timer_handler_h
#define timer_handler_h

#include "scheduler.h"

#define TIMER_HANDLED -1.0	

class TimerHandler : public Handler {
public:
	TimerHandler() : status_(TIMER_IDLE) { }

	void sched(double delay);	// cannot be pending
	void resched(double delay);	// may or may not be pending
					// if you don't know the pending status
					// call resched()
	void cancel();			// must be pending
	inline void force_cancel() {	// cancel!
		if (status_ == TIMER_PENDING) {
			_cancel();
			status_ = TIMER_IDLE;
		}
	}
	enum TimerStatus { TIMER_IDLE, TIMER_PENDING, TIMER_HANDLING };
	int status() { return status_; };

protected:
	virtual void expire(Event *) = 0;  // must be filled in by client
	// Should call resched() if it wants to reschedule the interface.

	virtual void handle(Event *);
	int status_;
	Event event_;

private:
	inline void _sched(double delay) {
		(void)Scheduler::instance().schedule(this, &event_, delay);
	}
	inline void _cancel() {
		(void)Scheduler::instance().cancel(&event_);
		// no need to free event_ since it's statically allocated
	}
};

#endif /* timer_handler_h */
