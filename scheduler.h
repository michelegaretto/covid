/*
**  File:        scheduler.h
**  Program:     corona
**  Purpose:     class scheduler      
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#ifndef scheduler_h
#define scheduler_h

#include <string.h>
#include "heap.h"

#define	SCHED_START	0.0	/* start time (secs) */

class Handler;

class Event {
public:
	Event* next_;		/* event list */
	Handler* handler_;	/* handler to call when event ready */
	double time_;		/* time at which event is ready */
	long uid_;	/* unique ID */
	Event() : time_(0), uid_(0) {}
};

/*
 * The base class for all event handlers.  When an event's scheduled
 * time arrives, it is passed to handle which must consume it.
 * i.e., if it needs to be freed it, it must be freed by the handler.
 */
class Handler {
public:
	Handler() {}
	virtual void handle(Event* event) {}
};


class Scheduler {
public:

	Scheduler();
	virtual ~Scheduler();

	static Scheduler& instance() { return (*instance_); }
	
	inline double clock() const { return (clock_); }
	
	void schedule(Handler*, Event*, double delay);	// sched later event
	static void halt() { if(instance_) instance_->halted_ = 1; }
	virtual void run();			// execute the simulator
	virtual void cancel(Event*) = 0;	// cancel event
	virtual void insert(Event*) = 0;	// schedule event
	virtual Event* lookup(long uid) = 0;	// look for event
	virtual Event* deque() = 0;		// next event (removes from q)
	
	virtual void reset();
	
protected:
	void dumpq();	// for debug: remove + print remaining events
	void dispatch(Event*);	// execute an event
	void dispatch(Event*, double);	// exec event, set clock_
	
	double clock_;	// simulator virtual time
	int halted_;
	static Scheduler* instance_;
	static long uid_;
};

class ListScheduler : public Scheduler {
public:
	inline ListScheduler() : queue_(0) {}
	virtual void cancel(Event*);
	virtual void insert(Event*);
	virtual Event* deque();
	virtual Event* lookup(long uid);
protected:
	Event* queue_;
};

class HeapScheduler : public Scheduler {
public:
	HeapScheduler() { hp_ = new Heap; } 
	void cancel(Event* e) {
		if (e->uid_ <= 0)
			return;
		e->uid_ = - e->uid_;
		hp_->heap_delete((void*) e);
	}
	void insert(Event* e) {
		hp_->heap_insert(e->time_, (void*) e);
	}
	Event* lookup(long uid);
	Event* deque();
	const Event* head() { return (const Event *)hp_->heap_min(); }
protected:
	Heap* hp_;
};

#endif
