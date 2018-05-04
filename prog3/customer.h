/*-------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 3
*/

/* customer.h */
#ifndef CUSTOMER_H_
#define CUSTOMER_H_

typedef struct customer_s 
{
	double arrival_time; /* gets to the store */
	double enqueue_time; /* gets in line for cashier */
	double checkout_time;/* gets to cashier */
	double leave_time; /* leaves the store */
} customer_t;

#endif /* CUSTOMER_H_ */
/*------------------------*/
