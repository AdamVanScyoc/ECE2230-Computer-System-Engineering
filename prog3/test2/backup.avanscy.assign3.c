/*-----------------------------*/
/* avanscy.assign3.c */

#define DEF_Q_COUNT 9999 // default maximum number of queues, overridden w/ -q
#define DEF_E_SIZE 9999 // defailt event queue size, overridden w/ -e
#define MAX_CUSTOMERS 999 // maximum number of customers in store at a given time

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


int main(int argc, char *argv[])
{
	int returncode = 0, // the value that will be returned by main
		numevents = 0,// store the number of events that have been allocated so that they
			// can be freed later
		numcustomer = 0; // number of cusomers in store at given instant
	long numqueues = 0l, maxnumevents = 0l; // for taking in command-line arguements that 
	double curtime = 0.0; // the current time in the simulation
		// specify the number of queues or the max event queue size
	char * endptr = NULL; // for attempting to read in integer command line arguements
		// with strtol
	queue_t ** allQueues = NULL; // contains the dynamically-allocated array of queues
	priority_t * eventq = NULL; // stores the event queue
	event_t * nextEv = NULL;

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
	allQueues = calloc((numqueues ? numqueues : DEF_Q_COUNT), sizeof(queue_t));
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
		else
			allQueues[x]->queuenumber = x;
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
	event_t ** allEvents = calloc(2 /*todo: replace this w/ 1*/, sizeof(event_t *));
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
	allEvents[0]->queue_number = 0;
	// allocate first event's customer struct
	allEvents[0]->customer = malloc(sizeof(*allEvents[0]->customer));
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
				nextEv->customer->enqueue_time = randsim_gauss(); // set enqueu time
				// schedule enqueue event

				// resize dymanically-allocated array storing all the events,
					// and increment numevents
				allEvents = realloc(allEvents, ++numevents*sizeof(event_t *));
				if (!allEvents)
				{
					// abandon hope
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
				
				allEvents[numevents - 1]->type = EV_ENQUEUE;
				allEvents[numevents - 1]->event_time = nextEq->customer->enqueue_time;
				//todo: add logic to determine shortest queue
				allEvents[numevents - 1]->queue_number = 1;

				// allocate newly scheduled EV_ENQUEUE event's customer struct
				allEvents[numevents - 1]->customer = malloc(sizeof(*allEvents[numevents - 1]->customer));
				if (!allEvents[numevents - 1]->customer)
				{
					printf("Fatal Error: when attempting to allocate first event's custoemr,");
					printf(" malloc returned NUL! terminating...\n");
					returncode = -1;
					goto finalize;
				}	

				// insert new EV_ENQUEUE event
				if (-1 == priority_insert(eventq, allEvents[numevents - 1]))
				{
					printf("Fatal Error: Could not insert new event! terminating...\n");
					returncode = -1;
					goto finalize;
				}
				
				// schedule the next customer to arrive if store isn't full
				if (numcustomers + 1 < MAX_CUSTOMERS)
				{
					numcustomers++;

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

					// populuate fields of new customer, prepare it for EV_ARRIVE event
					allEvents[numevents - 1]->type = EV_ARRIVE;
					allEvents[numevents - 1]->event_time = randsim_gauss();
					// allocate newly scheduled EV_ARRIVE event's customer struct
					allEvents[numevents - 1]->customer = malloc(sizeof(*allEvents[numevents - 1]->customer));
					if (!allEvents[numevents - 1]->customer)
					{
						printf("Fatal Error: when attempting to allocate first event's custoemr,");
						printf(" malloc returned NUL! terminating...\n");
						returncode = -1;
						goto finalize;
					}	

					// insert new EV_ARRIVE event
					if (-1 == priority_insert(eventq, allEvents[numevents - 1]))
					{
						printf("Fatal Error: Could not insert new event! terminating...\n");
						returncode = -1;
						goto finalize;
					}
					

				} // end if (++numcustomers < MAX_CUSTOMERS)

				break;
			} // end case EV_ARRIVE
			case EV_ENQUEUE:
			{
				break;
			} // end case EV_ENQUEUE
			case EV_CHECKOUT:
			{
				break;
			} // end case EV_CHECKOUT
			case EV_LEAVE:
			{
				break;
			} // end case EV_LEAVE

		} // end switch (nextEv->type)

		//todo: delete
		break;
	} // end while (!priority_empty(pq)
	
/* free/finalize all data */
finalize:
	// finalize all queues 
	for (int x = 0; x < (numqueues ? numqueues : DEF_Q_COUNT); x++)
	{
		queue_finalize(allQueues[x]);
	} // end for (int x = 0;)

	// free the allQueues double pointer that previously contained the array of queues
	if (allQueues)
		free(allQueues);

	// finalize event queue
	priority_finalize(eventq);

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
	
	return (returncode);
}
