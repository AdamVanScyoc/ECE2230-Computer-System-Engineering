/*----------------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 3
*/

/* customer.c */

#include "customer.h"

struct customer_s
{
	double arrival_time; /* gets to the store */
	double enqueue_time; /* gets in line for cashier */
	double checkout_time;/* gets to cashier */
	double leave_time; /* leaves the store */
};

/*--------------------------*/
