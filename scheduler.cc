/*
**  File:        scheduler.cc
**  Program:     VOD
**  Purpose:     class scheduler      
**  Author:      Michele Garetto
**  Last Update: November 5 2010 
*/

#include <stdlib.h>
#include <stdio.h>

#include "scheduler.h"

Scheduler::Scheduler() : clock_(SCHED_START), halted_(0)
{
}

Scheduler::~Scheduler(){
	instance_ = NULL ;
}

/*
 * Schedule an event delay time units into the future.
 * The event will be dispatched to the specified handler.
 * We use a relative time to avoid the problem of scheduling
 * something in the past.
 *
 */
void Scheduler::schedule(Handler* h, Event* e, double delay)
{
	// handler should ALWAYS be set... if it's not, it's a bug in the caller
	if (!h) {
		printf("Scheduler: attempt to schedule an event with a NULL handler.  Don't DO that.\n");
		abort();
	};
	if (e->uid_ > 0) {
		printf("Scheduler: Event UID not valid!\n\n");
		abort();
	}
	
	if (delay < 0) {
		// You probably don't want to do this
		// (it probably represents a bug in your simulation).
		fprintf(stderr, "warning: ns Scheduler::schedule: scheduling event\n"
		"\twith negative delay (%f) at time %f.\n", delay, clock_);
	}

	if (uid_ < 0) {
		fprintf(stderr, "Scheduler: UID space exhausted!\n");
		abort();
	}
	e->uid_ = uid_++;
	e->handler_ = h;
	double t = clock_ + delay;

	e->time_ = t;
	insert(e);
}

void Scheduler::run()
{
	Event *p;
	instance_ = this;
	while (!halted_ && (p = deque())) {
		dispatch(p);
	}
}

/*
 * dispatch a single simulator event by setting the system
 * virtul clock to the event's timestamp and calling its handler.
 * Note that the event may have side effects of placing other items
 * in the scheduling queue
 */

void Scheduler::dispatch(Event* p, double t)
{
	if (t < clock_) {
		fprintf(stderr, "ns: scheduler going backwards in time from %f to %f.\n", clock_, t);
	}

	clock_ = t;
	p->uid_ = -p->uid_;	// being dispatched
	p->handler_->handle(p);	// dispatch
}

void Scheduler::dispatch(Event* p)
{
	dispatch(p, p->time_);
}

void Scheduler::reset()
{
	clock_ = SCHED_START;
}

void Scheduler::dumpq()
{
	Event *p;
	
	if (!halted_) {
		fprintf(stderr, "Scheduler: dumpq only allowed while halted\n");
		return;
	}

	printf("Contents of scheduler queue (events) [cur time: %f]---\n",clock());
	while ((p = deque()) != NULL) {
		printf("t:%f uid: %ld", p->time_,p->uid_);
		printf(" handler: %p\n", p->handler_);
	}
}

void ListScheduler::insert(Event* e)
{
	double t = e->time_;
	Event** p;
	for (p = &queue_; *p != 0; p = &(*p)->next_)
		if (t < (*p)->time_)
			break;	
	e->next_ = *p;
	*p = e;
}

/*
 * Cancel an event.  It is an error to call this routine
 * when the event is not actually in the queue.  The caller
 * must free the event if necessary; this routine only removes
 * it from the scheduler queue.
 */
void ListScheduler::cancel(Event* e)
{
	Event** p;
	if (e->uid_ <= 0)	// event not in queue
		return;
	for (p = &queue_; *p != e; p = &(*p)->next_)
		if (*p == 0)
			abort();

	*p = (*p)->next_;
	e->uid_ = - e->uid_;
}

Event* ListScheduler::lookup(long uid)
{
	Event* e;
	for (e = queue_; e != 0; e = e->next_)
		if (e->uid_ == uid)
			break;
	return (e);
}


Event* ListScheduler::deque()
{ 
	Event* e = queue_;
	if (e)
		queue_ = e->next_;
	return (e);
}



/* HEAP STUFF */


Heap::Heap(int size): h_s_key(0), h_size(0), h_maxsize(size), h_iter(0)
{
	h_elems = new Heap_elem[h_maxsize];
	memset(h_elems, 0, h_maxsize*sizeof(Heap_elem));
}

Heap::~Heap()
{
	delete [] h_elems;
}

int Heap::heap_member(void* elem)
{
	unsigned int i;
	Heap::Heap_elem* he;
	for (i = 0, he = h_elems; i < h_size; i++, he++)
		if (he->he_elem == elem)
			return ++i;
	return 0;
}

/*
 * int	heap_delete(Heap *h, void *elem):		O(n) algorithm
 *
 *	Returns 1 for success, 0 otherwise.
 *
 * find elem in h->h_elems[] using heap_member()
 *
 * To actually remove the element from the tree, first swap it to the
 * root (similar to the procedure applied when inserting a new
 * element, but no key comparisons--just get it to the root).
 *
 * Then call heap_extract_min() to remove it & fix the tree.
 * 	This process is not the most efficient, but we do not
 *	particularily care about how fast heap_delete() is.
 *	Besides, heap_member() is already O(n), 
 *	and is the dominating cost.
 *
 * Actually remove the element by calling heap_extract_min().
 * 	The key that is now at the root is not necessarily the
 *	minimum, but heap_extract_min() does not care--it just
 *	removes the root.
 */
int Heap::heap_delete(void* elem)
{
	int i;
	if ((i = heap_member(elem)) == 0)
		return 0;
	for (--i; i; i = parent(i)) {
		swap(i, parent(i));
	}
	(void) heap_extract_min();
	return 1;
}

/*
 * void	heap_insert(Heap *h, heap_key_t *key, void *elem)
 *
 * Insert <key, elem> into heap h.
 * Adjust heap_size if we hit the limit.
 * 
 *	i := heap_size(h)
 *	heap_size := heap_size + 1
 *	while (i > 0 and key < h[Parent(i)])
 *	do	h[i] := h[Parent(i)]
 *		i := Parent(i)
 *	h[i] := key
 */
void Heap::heap_insert(heap_key_t key, void* elem) 
{
	unsigned int	i, par;
	if (h_maxsize == h_size) {	/* Adjust heap_size */
		unsigned int osize = h_maxsize;
		Heap::Heap_elem *he_old = h_elems;
		h_maxsize *= 2;
		h_elems = new Heap::Heap_elem[h_maxsize];
		memcpy(h_elems, he_old, osize*sizeof(Heap::Heap_elem));
		delete []he_old;
	}

	i = h_size++;
	par = parent(i);
	while ((i > 0) && 
	       (KEY_LESS_THAN(key, h_s_key,
			      h_elems[par].he_key, h_elems[par].he_s_key))) {
		h_elems[i] = h_elems[par];
		i = par;
		par = parent(i);
	}
	h_elems[i].he_key  = key;
	h_elems[i].he_s_key= h_s_key++;
	h_elems[i].he_elem = elem;
	return;
};
		
/*
 * void *heap_extract_min(Heap *h)
 *
 *	Returns the smallest element in the heap, if it exists.
 *	NULL otherwise.
 *
 *	if heap_size[h] == 0
 *		return NULL
 *	min := h[0]
 *	heap_size[h] := heap_size[h] - 1   # C array indices start at 0
 *	h[0] := h[heap_size[h]]
 *	Heapify:
 *		i := 0
 *		while (i < heap_size[h])
 *		do	l = HEAP_LEFT(i)
 *			r = HEAP_RIGHT(i)
 *			if (r < heap_size[h])
 *				# right child exists =>
 *				#       left child exists
 *				then	if (h[l] < h[r])
 *						then	try := l
 *						else	try := r
 *				else
 *			if (l < heap_size[h])
 *						then	try := l
 *						else	try := i
 *			if (h[try] < h[i])
 *				then	HEAP_SWAP(h[i], h[try])
 *					i := try
 *				else	break
 *		done
 */
void* Heap::heap_extract_min()
{
	void*	min;				  /* return value */
	unsigned int	i;
	unsigned int	l, r, x;

	if (h_size == 0)
		return 0;
	min = h_elems[0].he_elem;
	h_elems[0] = h_elems[--h_size];
// Heapify:
	i = 0;
	while (i < h_size) {
		l = left(i);
		r = right(i);
		if (r < h_size) {
			if (KEY_LESS_THAN(h_elems[l].he_key, h_elems[l].he_s_key,
					  h_elems[r].he_key, h_elems[r].he_s_key))
				x= l;
			else
				x= r;
		} else
			x = (l < h_size ? l : i);
		if ((x != i) && 
		    (KEY_LESS_THAN(h_elems[x].he_key, h_elems[x].he_s_key,
				   h_elems[i].he_key, h_elems[i].he_s_key))) {
			swap(i, x);
			i = x;
		} else {
			break;
		}
	}
	return min;
}

Event* HeapScheduler::lookup(long uid)
{
	Event* e;
	
	for (e = (Event*) hp_->heap_iter_init(); e;
	     e = (Event*) hp_->heap_iter())
		if (e->uid_ == uid)
			break;
	return e;
}

Event* HeapScheduler::deque()
{
	return ((Event*) hp_->heap_extract_min());
}

