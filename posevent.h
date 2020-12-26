/*
**  File:        posevent.h
**  Program:     corona
**  Purpose:     class PosEvent      
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#ifndef posevent_h
#define posevent_h

#include "scheduler.h"

enum PosEventType {
	POSEVENTNULL = 0x0000,
	NEWINFECTION = 0x0001,
    STARTPRESYMP = 0x0010,
    ENDPRESYMP = 0x0011,
    ENDSICK = 0x0100,
    ENDINTE = 0x0110,
    ENDRESIDUAL = 0x1000
};

class PosEvent : public Event {
friend class Pos;
friend class Virus;
public:
	
	PosEvent() : next_(0), type_(POSEVENTNULL) { }
	
	inline int type() { return (type_); }
	inline unsigned long id() { return (id_); }
		
	inline PosEvent* copy() const;
	static inline PosEvent* alloc();
	static inline PosEvent* alloc(int);
	static inline PosEvent* alloc(int,long);
	static inline void free(PosEvent*);
		
	PosEvent* next_; // for the free list

protected:

	static PosEvent* free_; // free list
	long id_;               // pos id
	
private:

	int type_;		// Event type 
};

inline PosEvent* PosEvent::alloc()
{
	PosEvent* pe = free_;
	if (pe != 0) {
		free_ = pe->next_;
		pe->uid_ = 0;
		pe->time_ = 0;
	} else {
		pe = new PosEvent;
	}
	pe->next_ = 0;
	pe->type_ = POSEVENTNULL;
	pe->id_ = -1;
	return (pe);
}

/* allocate a PosEvent of a given type */
inline PosEvent* PosEvent::alloc(int t)
{
	PosEvent* pe = alloc();
	pe->type_ = t; 
	return (pe);
}

/* allocate a PosEvent of a given type for a given id */
inline PosEvent* PosEvent::alloc(int t, long id)
{
	PosEvent* pe = alloc(t);
	pe->id_ = id; 
	return (pe);
}


inline void PosEvent::free(PosEvent* pe)
{
	pe->next_ = free_;
	free_ = pe;
}

inline PosEvent* PosEvent::copy() const
{
	
	PosEvent* pe = alloc();
	pe->type_ = this->type_;
	pe->id_ = this->id_;
	return (pe);
}

#endif
