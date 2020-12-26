/*
**  File:        service.h
**  Program:     corona
**  Purpose:     service routines    
**  Author:      Michele Garetto
**  Last Update: 2020 
*/

#include <string.h>
#include <math.h>
#include "service.h"
#include <time.h>


int get_int()
{
    static char linebuffer[255];
    char* pin;
    int var;
    do{
        pin = fgets(linebuffer, 255, stdin);      /*  Gets a data from stdin without flushing problems */
        pin = strrchr (linebuffer, '\n');                  
        if (pin!=NULL) *pin = '\0';
    } while( *linebuffer=='#' || (*linebuffer=='/' && *(linebuffer+1)=='/') || *linebuffer=='\0');
    sscanf(linebuffer,"%d",&var);
    return(var); 
}

int get_ints(int* v_)
{
    char linebuffer[255];
    char* pch;
    char* pin;
    int var,ret;
    int num = 0;
    do{
        pin = fgets(linebuffer, 255, stdin);      /*  Gets a data from stdin without flushing problems */
        pin = strrchr (linebuffer, '\n');                  
        if (pin!=NULL) *pin = '\0';
    } while( *linebuffer=='#' || (*linebuffer=='/' && *(linebuffer+1)=='/') || *linebuffer=='\0');
    // printf ("Splitting string \"%s\" into tokens:\n",linebuffer);
    pch = strtok(linebuffer," ,;");
    do{
      // printf ("Now I considered piece %s\n",pch);
      ret = sscanf(pch,"%d",&var);
      if(ret > 0) {
        v_[num] = var;
        num++;
        pch = strtok (NULL, " ,.-");
      }      
    } while (ret > 0);    
    return(num);
}

unsigned long get_long()
{
    static char linebuffer[255];
    char* pin;
    unsigned long var;
    do{
        pin = fgets(linebuffer, 255, stdin);      /*  Gets a data from stdin without flushing problems */
        pin = strrchr (linebuffer, '\n');                  
        if (pin!=NULL) *pin = '\0';
    } while( *linebuffer=='#' || (*linebuffer=='/' && *(linebuffer+1)=='/') || *linebuffer=='\0');
    sscanf(linebuffer,"%ld",&var);
    return(var); 
}

double get_double()
{
    static char linebuffer[255];
    char* pin;
    double var;
    do{
        pin = fgets(linebuffer, 255, stdin);      /*  Gets a data from stdin without flushing problems */
        pin = strrchr (linebuffer, '\n');                  
        if (pin!=NULL) *pin = '\0';
    } while( *linebuffer=='#' || (*linebuffer=='/' && *(linebuffer+1)=='/') || *linebuffer=='\0');
    sscanf(linebuffer,"%lf",&var);
    return(var); 
}

void get_string(char *s)
{
    static char linebuffer[255];
    char* pin;
    do{
        pin = fgets(linebuffer, 255, stdin);      /*  Gets a data from stdin without flushing problems */
        pin = strrchr (linebuffer, '\n');                  
        if (pin!=NULL) *pin = '\0';
    } while( *linebuffer=='#' || (*linebuffer=='/' && *(linebuffer+1)=='/') || *linebuffer=='\0');
    sscanf(linebuffer,"%s",s);
}

void error(const char*s)
{
  printf("Error : %s\n",s);
  exit(1);
}

void computeconf(double *b, double *min, double *mean, double *max)
{
	double tmp;
	int i;
	double var = 0.0;
	double ave = 0.0;
	
	ave = 0.;
	for (i = 1; i<=NUMBATCHES; i++) ave += b[i];
	ave /= NUMBATCHES;
	
	for (i = 1; i<=NUMBATCHES; i++) var += ((b[i]-ave)*(b[i]-ave));
	var = sqrt(var/(NUMBATCHES-1));
	tmp  = QUANTILE/sqrt(NUMBATCHES);
	(*min) = ave - var*tmp;
 	(*max) = ave + var*tmp;
	(*mean) = ave;
}


double RNG::triangular(double a, double b, double c) {
    double Fc = (c-a)/(b-a);
    double u = RNG::uniform();
    double ret;
    if(u < Fc) ret = a + sqrt(u*(b-a)*(c-a));
    else ret = b - sqrt((1-u)*(b-a)*(b-c));
    return(ret);
}    

/* general random variable specified by mean and variation coefficient 	*/
/* synthetized by H2 (c > 1) or D+M (c < 1) 				*/
/* parameters t1 and t2 are pre-computed once and for all 		*/
double RNG::rangeneral(double m, double c, double t1, double t2) 
{
	double ret = 0.0;
	if( c > 1.0 ) {
		double unif = uniform();
		double p = m/(2*t1);
		if(unif < p) ret = exponential(t1);
		else ret = exponential(t2);
	} else {
		ret = t1 + exponential(t2);
	}
	return(ret);
}					

void RNG::inputseed() 
{

  printf("Random number generator seed: ");
  seed_ = get_long();
  printf("%ld\n",seed_);
  
#ifdef SEEDFROMCLOCK
  time_t now = time(NULL);
  if (now == -1) error("The time() function failed");
  seed_ = (unsigned long int)now;
  printf("Using seed %ld from time\n",seed_);
#endif  

  if (seed_ > 0L) gsl_rng_set(rng_,seed_);
  
}  

