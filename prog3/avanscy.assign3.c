/*-----------------------------*/
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 3
*/

/* avanscy.assign3.c */

#define DEF_Q_COUNT 9999 // default maximum number of queues, overridden w/ -q
#define DEF_E_SIZE 9999 // default event queue size, overridden w/ -e
#define MAX_CUSTOMERS 99999 // maximum number of customers in store at a given time

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "customer.h"
#include "event.h"
#include "list.h"
#include "priority.h"
#include "queue.h"
#include "randsim.h"

void printstats(customer_t ** cust, double numcustomers, double numqueues, double numevents);

int main(int argc, char *argv[])
{
	int returncode = 0, // the value that will be returned by main
		numevents = 0,// store the number of events that have been allocated so that they
			// can be freed later
		numcustomers = 0; // number of cusomers in store at given instant
	long numqueues = 0l, maxnumevents = 0l; // for taking in command-line arguements that 
	// specify the number of queues or the max event queue size
	double curtime = 0.0; // the current time in the simulation
	char * endptr = NULL; // for attempting to read in integer command line arguements
		// with strtol
	queue_t ** allQueues = NULL; // contains the dynamically-allocated array of queues
	priority_t * eventq = NULL; // stores the event queue
	event_t * nextEv = NULL; // stores the next event - the one that the event processing
		// loop pops off the PQ
	event_t ** allEvents = NULL; // stores all the events that get pushed onto the PQ so 
		// that they can be freed later
	customer_t ** allCustomers = NULL; // stores all the customers so that we can track each individually,
		// independent of any associated events, and free them later

/* process any command-line arguements */

	if (argc > 1)
	{
		int index = 1;
		while (index < argc)
		{
			// check for "-q" arguement - where the number of queues is provided
			// and check that there is an arguement following "-q" before trying to 
			// read in the number of queues
			if (!strcmp(argv[index], "-q"))
			{
				// make sure that an arguement was provided after the "-q"
				if (index + 1 == argc)
				{
					printf("Fatal Error: '-q' arguement specified but unable");
					printf(" to read-in the number of queues you provided. Terminating...\n");
					index++;
					// return failure
					return -1;
				} // end if (index + 1 == argc)
					
				// first, make sure that errno is not already set
				errno = 0;
				// attempt to convert argument following "-q" to an int; if endptr == the string we were trying to convert,
					// then that means that no digits were found and conversion failled
				numqueues = strtol(argv[index + 1], &endptr, 10);
				if (errno || endptr == argv[index + 1] || numqueues <= 0) // acceptable values for numqueues > 0
				{
					printf("Fatal Error: '-q' arguement specified but unable to read-in");
					printf(" the number of queues you provided. Terminating...\n %s ",
						 (errno ? strerror(errno) : ""));
					// return failure
					return -1;
				} // end if (errno)
					
			} // end if (!strcmp(argv[index], "-q")

			// check for "-q" arguement - where the max event queue size is provided
			// and check that there is an arguement following "-e" before trying to 
			// read in the max size
			else if (!strcmp(argv[index], "-e"))
			{
				// make sure that an arguement was provided after the "-e"
				if (index + 1 == argc)
				{
					printf("Fatal Error: '-e' arguement specified but unable to read-in");
					printf("  the maximum number of events you provided. Terminating...\n");
					index++;
					// return failure
					return -1;
				} // end if (index + 1 == argc)

				// first, make sure that errno is not already set
				errno = 0;
				// attempt to convert argument following "-e" to an int; if endptr == the string we were trying to conver,
					// then that means that no digits were found and conversion failled
				maxnumevents = strtol(argv[index + 1], &endptr, 10);
				if (errno || endptr == argv[index + 1] || maxnumevents <= 0) // acceptable values for maxnumevents > 0
				{
					printf("Fatal Error: '-e' arguement specified but unable to read-in");
					printf(" the maximum number of events you provided. Terminating...\n %s ",
						(errno ? strerror(errno) : ""));
					// return failure
					return -1;
				} // end if (errno)
					
			} // end if (!strcmp(argv[index], "-q")

			index++;
		} // end while (index < argc - 1)
	} // end if (argc > 1)	

/* done processing command line arguments */

/* initialize queues */

	// if number of queues was specified on command line, use that number. otherwise use default number
	allQueues = calloc((numqueues ? numqueues : DEF_Q_COUNT), sizeof(queue_t *));
	if (!allQueues)
	{
		printf("Fatal Error: When attempting to allocate for allQueues, malloc returned NULL! terminating...\n");
		returncode = -1; // return failure
		goto finalize; // attempt to exit gracefully
	}
	for (int x = 0; x < (numqueues ? numqueues : DEF_Q_COUNT); x++)
	{
		// initialize a new queue with max size MAX_Q_SIZE
		allQueues[x] = queue_init(MAX_Q_SIZE); 
		if (!allQueues[x])	
		{
			printf("Fatal Error: initialization of all the queues failled! terminating...\n");
			returncode = -1; // return failure
			goto finalize; // exit gracefully
		}
	} // end for (int x = 0;)

/* initialize event (priority) queue */
	// if '-e' arguement was specified and default event queue size was over-ridden,
	//  use that size instead
	eventq = priority_init(( maxnumevents ? maxnumevents : DEF_E_SIZE ));
	if (!eventq)
	{
		printf("Fatal Error: initialization of the event queue failled! terminating...\n");
		goto finalize; // exit gracefully
	}
	
/* malloc new event and customer, schedule arrive event at t=0 and put in event queue */
	// initially, allocate space for a single event
	allEvents = calloc(1, sizeof(event_t *));
	if (!allEvents)
	{
		printf("Fatal Error: when attempting to allocate allEvents, malloc returned NULL! terminating...\n");
		returncode = -1;
		goto finalize;
	}
	allEvents[0] = calloc(1, sizeof(*allEvents[0]));
	numevents++; // keeping track of how many events we've allocated so we can free them all
	if (!allEvents[0])
	{
		printf("Fatal Error: when attempting to allocate first event, malloc returned NULL! terminating...\n");
		numevents--; // numevents shoulld reflect that this alloc failled
		returncode = -1;
		goto finalize;
	}
	//set first event as the arrival of the first customer at t=0
	allEvents[0]->type = EV_ARRIVE;
	allEvents[0]->event_time = 0.0;
	allEvents[0]->queue_number = -1;
	
	// allocate the dynamically-allocated array containing all the customers
	allCustomers = calloc(1, sizeof(customer_t *));
	if (!allCustomers)
	{
		printf("Fatal Error: when attempting to allocate all customers, calloc returned NULL! terminating...\n");
		returncode = -1;
		goto finalize;
	} // end if (!allCustomers)

	// now allocate first customer struct
	allCustomers[0] = calloc(1, sizeof(customer_t));
	if (!allCustomers[0])
	{
		printf("Fatal Error: when attempting to allocate first customer, calloc returned NULL! terminating...\n");
		returncode = -1;
		goto finalize;
	} // end of (!allCustomers[0])
	else
		numcustomers++;

	
	allEvents[0]->customer = allCustomers[0];
	if (!allEvents[0]->customer)
	{
		printf("Fatal Error: when attempting to allocate first event's custoemr,");
		printf(" malloc returned NUL! terminating...\n");
		returncode = -1;
		goto finalize;
	}	
	allEvents[0]->customer->arrival_time = 0.0;

	// insert first event
	if (-1 == priority_insert(eventq, allEvents[0]))
	{
		printf("Fatal Error: Could not insert first event! terminating...\n");
		printf("Did you specify a large enough Event Queue size?\n");
		returncode = -1;
		goto finalize;
	}

/* main event processing while loop */

	// event handling while loop
	while (!priority_empty(eventq))
	{
		nextEv = priority_remove(eventq);
		if (!nextEv) // failed to pop next event from PQ
			break; // break loop; either failure, or no more events

		curtime = nextEv->event_time;
		
		//event-processing switch statement
		switch (nextEv->type)
		{
			case EV_ARRIVE:
			{
				// set arrival time to the current time for use in statatistics later
				nextEv->customer->arrival_time = curtime;

				// schedule enqueue event

				// resize dymanically-allocated array storing all the events,
				// and increment numevents
				allEvents = realloc(allEvents, ++numevents*sizeof(event_t *));
				if (!allEvents)
				{
					printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
					numevents--; // numevents shoulld reflect that this alloc failled
					returncode = -1;
					goto finalize;
				} // end if (!allEvents[numevents - 1])

				// allocate new event
				allEvents[numevents - 1] = calloc(1, sizeof(event_t));
				if (!allEvents[numevents - 1])
				{
					printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
					numevents--; // numevents shoulld reflect that this alloc failled
					returncode = -1;
					goto finalize;
				} // end if (!allEvents[numevents - 1])

				// the new enqueue event has the same customer as the current arrive event
				// populuate fields of new event
				allEvents[numevents - 1]->customer = nextEv->customer;
				allEvents[numevents - 1]->type = EV_ENQUEUE;
				allEvents[numevents - 1]->event_time = TIME_SHOP()+ curtime;

				// insert new EV_ENQUEUE event
				if (-1 == priority_insert(eventq, allEvents[numevents - 1]))
				{
					printf("Fatal Error: Could not insert new event! terminating...\n");
					printf("Did you specify a large enough Event Queue size?\n");
					returncode = -1;
					goto finalize;
				}
				
				// schedule the next customer to arrive if store isn't full
				if (numcustomers + 1 <= MAX_CUSTOMERS)
				{
					// resize dymanically-allocated array storing all the events,
					// and increment numevents
					allEvents = realloc(allEvents, ++numevents*sizeof(event_t *));
					if (!allEvents)
					{
						printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
						numevents--; // numevents shoulld reflect that this alloc failled
						returncode = -1;
						goto finalize;
					} // end if (!allEvents[numevents - 1])

					// allocate new event
					allEvents[numevents - 1] = calloc(1, sizeof(event_t));
					if (!allEvents[numevents - 1])
					{
						printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
						numevents--; // numevents shoulld reflect that this alloc failled
						returncode = -1;
						goto finalize;
					} // end if (!allEvents[numevents - 1])

					// populuate fields of new event
					allEvents[numevents - 1]->type = EV_ARRIVE;
					allEvents[numevents - 1]->event_time = TIME_ARRIVE() + curtime;

					// increase allCUstomers array size	
					allCustomers = realloc(allCustomers, ++numcustomers*sizeof(customer_t *));
					if (!allCustomers)
					{
						printf("Fatal Error: when attempting to allocate all customers, realloc returned NULL! terminating...\n");
						returncode = -1;
						numcustomers--;
						goto finalize;
					} // end if (!allCustomers)

					allCustomers[numcustomers - 1] = calloc(1, sizeof(customer_t));
					if (!allCustomers[numcustomers - 1])
					{
						printf("Fatal Error: when attempting to allocate new customer, calloc returned NULL! terminating...\n");
						numcustomers--;
						returncode = -1;
						goto finalize;
					} // end of (!allCustomers[0])
					
					allEvents[numevents - 1]->customer = allCustomers[numcustomers - 1];

					// insert new EV_ARRIVE event
					if (-1 == priority_insert(eventq, allEvents[numevents - 1]))
					{
						printf("Fatal Error: Could not insert new event! terminating...\n");
						printf("Did you specify a large enough Event Queue size?\n");

						returncode = -1;
						goto finalize;
					}

				} // end if (++numcustomers < MAX_CUSTOMERS)

				break;

			} // end case EV_ARRIVE

			case EV_ENQUEUE:
			{
				nextEv->customer->enqueue_time = curtime;

				// determine shortest queue
				int queuemin = queue_size(allQueues[0]); // start at first queue
				int indexofmin = 0;
				int isempty = 0; // if 1, means queue is empty so need to schedule checkout event
				queue_t * queueminloc = NULL; // pointer to shortest queue
		
				for (int z = 0; z <= ( numqueues ? numqueues - 1 : DEF_Q_COUNT ); z++)
				{
					// check every queue until we find the shortest; if we find an empty queue, break loop
					if (queue_size(allQueues[z]) < queuemin)
					{
						queuemin = queue_size(allQueues[z]);
						indexofmin = z;
					}
					if (!queue_size(allQueues[z])) // queue is empty
					{
						indexofmin = z;
						isempty = 1;
						break;	
					}
				} // end for (int z = 0;
				
				queueminloc = allQueues[indexofmin];

				// insert customer into shortest queue
				if (-1 == queue_insert(queueminloc, nextEv->customer))
				{
					printf("Fatal Error: Could not insert new customer! terminating...\n");
					returncode = -1;
					goto finalize;
				}

				nextEv->queue_number = indexofmin + 1;
		
				// if the queue is empty, schedule a checkout event
				if (isempty)
				{
					// resize dymanically-allocated array storing all the events,
					// and increment numevents
					allEvents = realloc(allEvents, ++numevents*sizeof(event_t *));
					if (!allEvents)
					{
						printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
						numevents--; // numevents shoulld reflect that this alloc failled
						returncode = -1;
						goto finalize;
					} // end if (!allEvents[numevents - 1])

					// allocate new event
					allEvents[numevents - 1] = calloc(1, sizeof(event_t));
					if (!allEvents[numevents - 1])
					{
						printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
						numevents--; // numevents shoulld reflect that this alloc failled
						returncode = -1;
						goto finalize;
					} // end if (!allEvents[numevents - 1])
					
					allEvents[numevents - 1]->type = EV_CHECKOUT;
					allEvents[numevents - 1]->customer = nextEv->customer; // new checkout event has same customer as current enqueue event
					allEvents[numevents - 1]->event_time = curtime; // should take place w/ no delay
					allEvents[numevents - 1]->queue_number = indexofmin + 1;
					// queue number of queue we're inserting into = index of shortest queue we found earlier + 1

					// insert new EV_CHECKOUT event
					if (-1 == priority_insert(eventq, allEvents[numevents - 1]))
					{
						printf("Fatal Error: Could not insert new event! terminating...\n");
						printf("Did you specify a large enough Event Queue size?\n");
						returncode = -1;
						goto finalize;
					}
				} // end if (isempty)
				
				break;
			} // end case EV_ENQUEUE

			case EV_CHECKOUT:
			{
				nextEv->customer->checkout_time = curtime;

				// save the queuenumber of the customer associated w/ EV_CHECKOUT event
				int thisqueue = nextEv->queue_number;
				
				// peek the next customer from particular queue
				// but don't remove them until EV_LEAVE event occurs
				customer_t * thiscust = queue_peek(allQueues[thisqueue - 1]);

				// resize dymanically-allocated array storing all the events,
				// and increment numevents
				allEvents = realloc(allEvents, ++numevents*sizeof(event_t *));
				if (!allEvents)
				{
					printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
					numevents--; // numevents shoulld reflect that this alloc failled
					returncode = -1;
					goto finalize;
				} // end if (!allEvents[numevents - 1])

				// allocate new event
				allEvents[numevents - 1] = calloc(1, sizeof(event_t));
				if (!allEvents[numevents - 1])
				{
					printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
					numevents--; // numevents shoulld reflect that this alloc failled
					returncode = -1;
					goto finalize;
				} // end if (!allEvents[numevents - 1])
				
				allEvents[numevents - 1]->type = EV_LEAVE;
				allEvents[numevents - 1]->customer = thiscust; // new leave event has same customer as current CHECKOUT event
				allEvents[numevents - 1]->event_time = TIME_CHECKOUT() + curtime; // the length of time that a customer takes to CHECKOUT is random
				allEvents[numevents - 1]->queue_number = nextEv->queue_number; // same queue as current EV_CHECKOUT event

				// insert new EV_LEAVE event
				if (-1 == priority_insert(eventq, allEvents[numevents - 1]))
				{
					printf("Fatal Error: Could not insert new event! terminating...\n");
					printf("Did you specify a large enough Event Queue size?\n");

					returncode = -1;
					goto finalize;
				}
				break;
			} // end case EV_CHECKOUT
			case EV_LEAVE:
			{
				nextEv->customer->leave_time = curtime;
	
				// now ready to remove this customer from the queue
				queue_remove(allQueues[nextEv->queue_number - 1]);

				// this queue is now ready to check out another customer
				if (1 <= queue_size(allQueues[nextEv->queue_number - 1]))
				{
				 	// customers remain in this queue
					customer_t * nextCust = queue_peek(allQueues[nextEv->queue_number - 1]);
					if (!nextCust)
					{
						// even though customers remain in queue, failled to retrieve the next one; fatal error
						printf("Fatal Error: cannot access next customer in queue! terminating...\n");
						returncode = -1;
						goto finalize;
					} // end if (!nextCust)
						
					// resize dymanically-allocated array storing all the events,
					// and increment numevents
					allEvents = realloc(allEvents, ++numevents*sizeof(event_t *));
					if (!allEvents)
					{
						printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
						numevents--; // numevents shoulld reflect that this alloc failled
						returncode = -1;
						goto finalize;
					} // end if (!allEvents[numevents - 1])

					// allocate new event
					allEvents[numevents - 1] = calloc(1, sizeof(event_t));
					if (!allEvents[numevents - 1])
					{
						printf("Fatal Error: when attempting to allocate new event, malloc returned NULL! terminating...\n");
						numevents--; // numevents shoulld reflect that this alloc failled
						returncode = -1;
						goto finalize;
					} // end if (!allEvents[numevents - 1])
					
					allEvents[numevents - 1]->type = EV_CHECKOUT;
					allEvents[numevents - 1]->customer = nextCust; // new checkout event has same customer as the customer we just removed from queue
					allEvents[numevents - 1]->event_time = curtime; // should take place w/ no delay
					allEvents[numevents - 1]->queue_number = nextEv->queue_number;

					// insert new EV_CHECKOUT event
					if (-1 == priority_insert(eventq, allEvents[numevents - 1]))
					{
						printf("Fatal Error: Could not insert new event! terminating...\n");
						printf("Did you specify a large enough Event Queue size?\n");


						returncode = -1;
						goto finalize;
					}
					
				} // end if (1 <= queue_size(allQueues[nextEv->queue_number - 1]))

				break;
			} // end case EV_LEAVE

		} // end switch (nextEv->type)

		
		if (nextEv)
		{
			free(nextEv);
			nextEv = NULL;
		} // end if (nextEv)

	} // end while (!priority_empty(pq)
	
/* print overall stats*/
	printstats(allCustomers, (double)numcustomers, (double)(numqueues ? numqueues : DEF_Q_COUNT), (double)(maxnumevents ? maxnumevents : DEF_E_SIZE));


/* free/finalize all data */
finalize:
	// free the temp event used in event processing loop
	if (nextEv)
	{
		free(nextEv);
	}

	// finalize all queues 
	if (allQueues)
	{
		for (int x = 0; x < (numqueues ? numqueues : DEF_Q_COUNT); x++)
		{
			if (allQueues[x])
				queue_finalize(allQueues[x]);
		} // end for (int x = 0;)
	}

	// finalize event queue
	priority_finalize(eventq);

	// free the allQueues double pointer that previously contained the array of queues
	if (allQueues)
		free(allQueues);


	// free all the events we've allocated
	if (allEvents)
	{
		for (int y = 0; y < numevents; y++)
		{
			if (allEvents[y])
				free(allEvents[y]);
		}

		free(allEvents);
	} // end if (allEvents)

	// free all customers
	if (allCustomers)
	{
		for (int k = 0; k < numcustomers; k++)
		{
			if (allCustomers[k])
				free(allCustomers[k]);
		}
		free(allCustomers);
	} 
	
	return (returncode);
}

void printstats(customer_t ** cust, double numcustomers, double numqueues, double numevents)
{		
	int x = 0;
	double linetime[3] = {0,0,0}; // min, max, and avg time in line (= checkout_time - enqueue_time)
	double tottime[3] = {0,0,0}; // min, max, and avg total time in store ( = leave_time)
	double avgchecktime = 0.0; // avg time spent checking out (leave_time - checkout_time)
	double avgshoptime = 0.0; // avg time spent shopping (enqueue_time - arrival_time)
	double meanarrival = 0.0; // mean of customer_s.arrival_time
	double meanenqueue = 0.0; // mean of customer_s.enqueue_time
	double meancheck = 0.0; // mean of customer_s.checkout_time
	double meanleave = 0.0; // mean of customer-s.leave_time

	if (!cust || numcustomers <= 0)
		return;

	// min time in line
	linetime[0] = cust[0]->checkout_time - cust[0]->enqueue_time;
	// max time in line
	linetime[1] = cust[0]->checkout_time - cust[0]->enqueue_time;

	tottime[0] = cust[0]->leave_time - cust[0]->arrival_time; //cust[0]->arrival_time + cust[0]->enqueue_time + cust[0]->checkout_time + cust[x]->leave_time;
	tottime[1] = cust[0]->leave_time - cust[0]->arrival_time; //cust[0]->arrival_time + cust[0]->enqueue_time + cust[0]->checkout_time + cust[0]->leave_time;

	// sum relevant values of all customers
	while (x < numcustomers)
	{
		if (!cust[x])
			continue;
		
		// avg time in line
		linetime[2] += cust[x]->checkout_time - cust[x]->enqueue_time;
		// min time in line
		if (linetime[0] > (cust[x]->checkout_time - cust[x]->enqueue_time))
			linetime[0] = cust[x]->checkout_time - cust[x]->enqueue_time;
		// max time in line
		if (linetime[1] < (cust[x]->checkout_time - cust[x]->enqueue_time))
			linetime[1] = cust[x]->checkout_time - cust[x]->enqueue_time;

		// avg total time
		tottime[2] += cust[x]->leave_time - cust[x]->arrival_time;
		// min total time
		if (tottime[0] > cust[x]->leave_time - cust[x]->arrival_time)
			tottime[0] = cust[x]->leave_time - cust[x]->arrival_time;
		// max total time
		if (tottime[1] <  cust[x]->leave_time - cust[x]->arrival_time)
			tottime[1] = cust[x]->leave_time - cust[x]->arrival_time;

		avgchecktime += cust[x]->leave_time - cust[x]->checkout_time;
		avgshoptime += cust[x]->enqueue_time - cust[x]->arrival_time;

		// comput the means of the customer_s fields
		meanarrival += cust[x]->arrival_time;
		meanenqueue += cust[x]->enqueue_time;
		meancheck += cust[x]->checkout_time;
		meanleave += cust[x]->leave_time;
		
		x++;
	} // end while (x < numcustomers) 

	// turn the sums into averages
	linetime[2] /= numcustomers;
	tottime[2] /= numcustomers;
	avgchecktime /= numcustomers;
	avgshoptime /= numcustomers;

	meanarrival /= numcustomers;
	meanenqueue /= numcustomers;
	meancheck /= numcustomers;
	meanleave /= numcustomers;
	
	// print stats
	printf("\n********** SIMULATION STATISTICS **********\n\n");
	printf("Number of Customers: %f\tNumber of Queues: %f\tMaximum Number of Events in Event Queue: %f\n", numcustomers, numqueues, numevents);
	printf("Minimum Time In Line: %f\tMaximum Time In Line: %f\tAverage Time In Line: %f\n", linetime[0], linetime[1], linetime[2]);
	printf("Average Time Spent Shopping: %f\n", avgshoptime);
	printf("Average Time Spent Checking Out: %f\n", avgchecktime);
	printf("Minimum Total Time: %f\tMaximum Total Time: %f\tAverage Total Time: %f\n", tottime[0], tottime[1], tottime[2]);
	printf("Mean Arrival Time: %f\n", meanarrival);
	printf("Mean Enqueue Time: %f\n", meanenqueue);
	printf("Mean Checkout Time: %f\n", meancheck);
	printf("Mean Leave Time: %f\n\n", meanleave);
}
