/*
**  File:        pos.h
**  Program:     corona
**  Purpose:     class Pos     
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#ifndef pos_h
#define pos_h

enum PosState { U_INCUB, K_INCUB, U_PRE_SYMP, K_PRE_SYMP, U_SYMP, K_SYMP, U_SUPER, K_SUPER, U_ASYMP, K_ASYMP, U_RECOVERED, K_RECOVERED, U_DEAD, K_DEAD, U_RESIDUAL, K_RESIDUAL, U_INTENSIVE, K_INTENSIVE };

class Pos {

public:

	Pos() : arrivaltime_(-1), id_(-1), next_(0), prev_(0) { }
	
	inline Pos* copy() const;
	static inline Pos* alloc();
	static inline Pos* alloc(long);
	static inline void free(Pos*);
	static const char* statestring(int state) { return StateString[state]; } 
		
	double arrivaltime_;
	double incubtime_;
	double symptime_;
	PosState state_;
	long id_;
	long revindex_;
	
	Pos* next_;
	Pos* prev_;

protected:

	static Pos* free_; // free list
	static const char StateString[18][20]; 
};

inline Pos* Pos::alloc()
{
	Pos* r = free_;
	if (r != 0) {
		free_ = r->next_;
	} else {
		r = new Pos;
	}
	r->next_ = 0;
	r->prev_ = 0;
	return (r);
}

/* allocate a pos with given id */
inline Pos* Pos::alloc(long id)
{
	Pos* r = alloc();
	r->id_ = id; 
	return (r);
}

/* free pos */
inline void Pos::free(Pos* r)
{
	r->next_ = free_;
	free_ = r;
}

#endif
