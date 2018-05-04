/*-----------------------------------*/
/* randsim.h */
#ifndef RANDSIM_H_
#define RANDSIM_H_

/* this uses log() from the math library, which might need the library */
/* linked in with -lm on the compile command line */

#define M1 1.0   /* median of the exponential function in minutes - ARRIVE */
#define M2 60.0  /* median of the gaussian function in minutes - SHOP */
#define D2 10.0  /* Std deviation of the gaussian function in minutes - SHOP */

/* Call all of the time macros with empty parenthesis */

/* Uniform Distribution */
double randsim_uni();

#define TIME_CHECKOUT() (randsim_uni() * 10.0)

/* Exponential Distribution */
double randsim_exp();

#define TIME_ARRIVE randsim_exp

/* Gaussian Distribution */
double randsim_gauss();

#define TIME_SHOP randsim_gauss

#endif /* RANDSIM_H_ */
/*-------------------------------*/
