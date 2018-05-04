/*-----------------------------*/
/* test.c */

#define DEF_Q_SIZE 99 // default queue size, overridden w/ -q
#define DEF_E_SIZE 99 // defailt event queue size, overridden w/ -e

// test driver for prog3
#include <stdio.h>
#include <stdlib.h>

#include "../customer.h"
#include "../event.h"
#include "../list.h"
#include "../priority.h"
#include "../queue.h"

void printEvent(event_t * event);

int main()
{
// test priority.c
//*
	// test priority_init()		
	priority_t * newPriority = priority_init(10);
	
	// test priority_insert()
	event_t * event1 = calloc(1, sizeof(*event1));
	event1->type = 1;
	event1->event_time = 0.005;
	event1->customer = NULL;
	event1->queue_number = 1;
	if (-1 == priority_insert(newPriority, 	event1))
	{
		printf("Error inserting 1st node! \n");
	}

	event_t * event2 = calloc(1, sizeof(*event2));
	event2->type = 2;
	event2->event_time = 0.005;
	event2->customer = NULL;
	event2->queue_number = 2;

	if (-1 == priority_insert(newPriority, 	event2))
	{
		printf("Error inserting 2nd node! \n");
	}

	event_t * event3 = calloc(1, sizeof(*event2));
	event3->type = 3;
	event3->event_time = 0.005;
	event3->customer = NULL;
	event3->queue_number = 3;

	if (-1 == priority_insert(newPriority, 	event3))
	{
		printf("Error inserting 3rd node! \n");
	}

	// test priority_remove()
	event_t * event4 = priority_remove(newPriority);
	if (event4)
		printEvent(event4);
	else
		printf("priorit_remove() returned NULL !\n");

	event_t * event5 = priority_remove(newPriority);
	if (event5)
		printEvent(event5);
	else
		printf("priorit_remove() returned NULL !\n");

	event_t * event6 = priority_remove(newPriority);
	if (event6)
		printEvent(event6);
	else
		printf("priorit_remove() returned NULL !\n");

	// test priority_finalize()
	priority_finalize(newPriority);

	// free everything
	free(event1);
	free(event2);
	free(event3);
	free(event4);
	free(event5);
	free(event6);
//*/

// test queue.c
/*
	queue_t * newQueue = queue_init(10);
	if (!newQueue)
	{
		printf("queue_init(10); return NULL\n");
	}
	
	customer_t * cust1 = malloc(sizeof(customer_t));
	cust1->arrival_time = 0.0001;
	cust1->enqueue_time = 0.0010;
	cust1->checkout_time = 0.01;
	cust1->leave_time = 0.1;

	if (-1 == queue_insert(newQueue, cust1))
		printf("queue_insert(newQueue, cust1) failled!\n");

	printf("\n Testing queue_full() and queue_size(): %d %d\n", 
		queue_full(newQueue), queue_size(newQueue));

	if (!queue_peek(newQueue))
		printf("Queue_peek(newQueue) returned NULL.\n");

	customer_t * cust2 = malloc(sizeof(customer_t));
	cust2->arrival_time = 0.0002;
	cust2->enqueue_time = 0.0020;
	cust2->checkout_time = 0.02;
	cust2->leave_time = 0.2;

	if (-1 == queue_insert(newQueue, cust2))
		printf("queue_insert(newQueue, cust2) failled!\n");

	printf("\n Testing queue_full() and queue_size(): %d %d\n", 
		queue_full(newQueue), queue_size(newQueue));

	if (!queue_peek(newQueue))
		printf("Queue_peek(newQueue) returned NULL.\n");

	customer_t * delCust = queue_peek(newQueue);
	while (0 < queue_size(newQueue))
	{
		delCust = queue_remove(newQueue);
		if (delCust)
			free (delCust);
	}	

	queue_finalize(newQueue);
*/
	return 0;
}

void printEvent(event_t * event)
{
	printf("\nEvent:\n");
	printf("event.type = %d\n", event->type);
	printf("event.event_time = %f\n", event->event_time);
	printf("event.customer = \n");
	printf("event.queue_number = %d\n\n", event->queue_number);
}
