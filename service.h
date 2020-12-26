/*
**  File:        service.h
**  Program:     corona
**  Purpose:     service routines    
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#ifndef service_h
#define service_h

#include <assert.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_poly.h>

#define MAXCAR 200
#define NUMBATCHES 30
#define QUANTILE   2.0452
#define PI M_PI

// #define MAXBINS 100000L
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
// #define SEEDFROMCLOCK

int get_int(); 
int get_ints(int*); 
unsigned long get_long();
double get_double();
void get_string(char *s); 
void error(const char*s);
void computeconf(double *b, double *min, double *mean, double *max);

/* Random number generator */
class RNG
{

public:
	
	static double uniform() { return(gsl_rng_uniform_pos(rng_)); }
	static double uniform(double a, double b) { return(a + gsl_rng_uniform_pos(rng_)*(b-a)); }
	static double triangular(double a, double b, double c); 
	static unsigned long uniformint(unsigned long a, unsigned long b) { return(a + gsl_rng_uniform_int(rng_,b-a+1)); }
	static int geom(double p) { return(gsl_ran_geometric(rng_,p)); }
	static int binom(double p, unsigned int n) { return(gsl_ran_binomial(rng_,p,n)); }
	static double lognormal(double z, double sigma) { return(gsl_ran_lognormal (rng_,z,sigma)); }
	static double exponential(double mu) { return(gsl_ran_exponential(rng_,mu)); } 
	static double stdgauss(double x) { return(gsl_ran_ugaussian_pdf(x)); } 
	static double poissonpdf(unsigned int k, double mu) { return(gsl_ran_poisson_pdf (k,mu)); }
	static unsigned int poisson(double mu) { return gsl_ran_poisson(rng_,mu); }
	static double rangeneral(double m, double c, double t1, double t2); 
	static double gaussian(double m, double sigma) { return(m + gsl_ran_gaussian(rng_,sigma)); }
	static double pareto(double a, double b) { return gsl_ran_pareto(rng_,a,b); }
	static long zipf(gsl_ran_discrete_t * zipf) { return((long)gsl_ran_discrete(rng_,zipf)); }
	static void inputseed();
	static inline unsigned long int getseed() { return(seed_); }
	
private:
	
	static gsl_rng* rng_;
	static unsigned long int seed_;      /* Random number generator seed */
};

#endif
