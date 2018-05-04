/*---------------------------------*/
/* randsim.c */

/* this uses log() from the math library, which might need the library */
/* linked in with -lm on the compile command line */

#include <stdlib.h>
#include <math.h>
#include "randsim.h"

/* NOTE TO PROGRAMMERS - you MUST put a function prototype in your code */
/* for each of the functions you plan to call. */

/* double M1 = 4.5;   median of the exponential function */
/* double M2 = 4.5;   median of the gaussian function */
/* double D2 = 2.2;   Std deviation of the gaussian function */

/* Uniform Distribution */
double randsim_uni()
{
	long int U = 0;
	while (U == 0)
		U = random();
	return ((double)U / (double)RAND_MAX);
}

/* Exponential Distribution */
double randsim_exp()
{
	return -M1 * log(randsim_uni());
}

static double rand_gas_full()
{
	static int iset = 0;
	static double gset;
	double fac, rsq, v1, v2;
	if (iset == 0) 
        {
		do {
			v1 = 2.0 * randsim_uni() - 1.0;
			v2 = 2.0 * randsim_uni() - 1.0;
			rsq = v1 * v1 + v2 * v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2.0 * log(rsq) / rsq);
		gset = v1 * fac * D2;
		iset = 1;
		return (v2 * fac * D2) + M2;
	} else {
		iset = 0;
		return gset + M2;
	}
}

/* Gaussian Distribution - prevents values outside of 2 STD DEVs */
double randsim_gauss()
{
	double u = -1.0;
	while(u < 0.0 || u > (2.0 * M2))
	{
		u = rand_gas_full();
	}
	return u;
}
