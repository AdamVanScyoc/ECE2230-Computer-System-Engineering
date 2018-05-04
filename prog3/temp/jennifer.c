/*	Jennifer Starcher
	ECE 2230-001
	Fall 2017
	main.c
	Purpose: The main simulation code.
*/

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include"randsim.h"
#include"sim.h"
#include"priority.h"
#include"list.h"
#include"queue.h"

#define MAX_LINE 10

void PrintStats(double Array[], int Length)
{
	double Avg = 0, Max = 0, Min = Array[0];
	for(int i = 0; i < Length; i++)
	{
		Avg = Avg + Array[i];
		if(Max < Array[i])
			Max = Array[i];
		if(Min > Array[i])
			Min = Array[i];
	}
	Avg = Avg/Length;
	printf("\t\tAverage: %0.2f\n\t\tMax: %0.2f\n\t\tMin: %0.2f\n", Avg, Max, Min);
}

int main(int argc, char *argv[])
{
	// Initialize Queues
	int i, min, Count = 0;
	double Time = 0;
	int num_customers = 0;

	if(argc != 5)
	{
		printf("ERROR: Enter Command Line Arguments.\n");
		exit(0);
	}
	if(atoi(argv[2]) run<= 0 || atoi(argv[4]) <= 0)
	{
		printf("ERROR: Enter Positive Number for Command Line Arguments.\n");
		exit(0);
	}
	int n, SizeOfEvents;
	if(strcmp(argv[1], "-q") == 0)
	{
		n = atoi(argv[2]);
		SizeOfEvents = atoi(argv[4]);
	}
	else if(strcmp(argv[1], "-e") == 0)
	{
		n = atoi(argv[4]);
		SizeOfEvents = atoi(argv[2]);
	}
	else
	{
		printf("ERROR: Enter Command Line Arguments.\n");
		exit(0);
	}
	queue_t *Q[n+1];
	for(i = 0; i <= n; i++)
	{
		Q[i] = queue_init(MAX_LINE);
		if(Q[i] == NULL)
			exit(0);
	}
	priority_t *Events = priority_init(SizeOfEvents); 
	
	double Shop[SizeOfEvents], Wait[SizeOfEvents], Overall[SizeOfEvents];

	// Malloc New Event and Customer
	event_t *event = (event_t*)malloc(sizeof(event_t));
	if(event == NULL)
		exit(0);
	event->customer = (customer_t*)malloc(sizeof(customer_t));
	if(event->customer == NULL)
		exit(0);

	// Now schedule arrive event at t=0 and put in event queue
	event->event_type = 1;
	event->event_time = 0;
	i = priority_insert(Events, event);
	num_customers++;

	event_t *event_next = (event_t*)malloc(sizeof(event_t));
	event_next->customer = (customer_t*)malloc(sizeof(customer_t));
	event_t *new_ev = (event_t*)malloc(sizeof(event_t));

	while(!(priority_empty(Events)))
	{
		new_ev = priority_remove(Events);
		if(new_ev == NULL)
			exit(0);
		Time = new_ev->event_time;
		printf("(%d)", new_ev->event_type);
		switch (new_ev->event_type)
		{
			case(1): // Arrival
				printf("Arrive %f\n", new_ev->event_time);
				new_ev->customer->arrival_time = Time;
				new_ev->event_time = Time + TIME_SHOP();
				new_ev->event_type = 2;
				i = priority_insert(Events, new_ev);
				if(i == -1)
					exit(0);
				/*if(SizeOfEvents > num_customers++)
				{
					// Malloc new event and customer
					//event_next = (event_t*)malloc(sizeof(event_t));
					//event_next->customer = (customer_t*)malloc(sizeof(customer_t));
					// Schedule an arrive event
					event_next->event_type = 1;
					event_next->event_time = TIME_ARRIVE();
					i = priority_insert(Events, event_next);
					if(i == -1)
						exit(0);
					num_customers++;
				}*/
				break;
			case(2): // Enqueue
				printf("Enqueue %f\n", new_ev->event_time);
				new_ev->customer->enqueue_time = Time;
				min = 1;
				for(i = 1; i <= n; i++)
				{
					if(queue_size(Q[i]) == 0)
					{
						min = i;
						break;
					}
					if(queue_size(Q[i]) < queue_size(Q[min]))
						min = i;
					
				}
				i = queue_insert(Q[min], new_ev->customer);
				if(i != 0)
					exit(0);
				new_ev->event_type = 3;
				new_ev->event_time = Time + 0;
				new_ev->queue_number = min;
				i = priority_insert(Events, new_ev);
				break;
			case(3): // Checkout
				printf("Checkout %f\n", new_ev->event_time);
				new_ev->customer = queue_peek(Q[new_ev->queue_number]);
				new_ev->customer->checkout_time = Time;
				new_ev->event_type = 4;
				new_ev->event_time = Time + TIME_CHECKOUT();
				i = priority_insert(Events, new_ev);
				break;
			case(4): // Leave
				printf("Leave %f\n", new_ev->event_time);
				i = new_ev->queue_number;
				new_ev->customer = queue_remove(Q[i]);
				new_ev->customer->leave_time = Time;
				if(queue_size(Q[i]) != 0)
				{
					new_ev->event_type = 4;
					new_ev->event_time = Time + TIME_CHECKOUT();
					i = priority_insert(Events, new_ev);
				}
				// Statistics
				Shop[Count] = new_ev->customer->enqueue_time - new_ev->customer->arrival_time;
				Wait[Count] = new_ev->customer->checkout_time - new_ev->customer->enqueue_time;
				Overall[Count] = new_ev->customer->leave_time - new_ev->customer->arrival_time;
				Count++;

				//free customer record?
				
				break;
			default:
				break;
		}
	}
	
	// Printf Statements
	printf("Number of Customers: %d\n", num_customers);
	printf("\tShop Time:\n");
	PrintStats(Shop, Count);
	printf("\tWait Time:\n");
	PrintStats(Wait, Count);
	printf("\tOverall Time:\n");
	PrintStats(Overall, Count);

	// Finalize
	free(new_ev);
	new_ev = NULL;
	free(event_next->customer);
	event_next->customer = NULL;
	free(event_next);
	event_next = NULL;
	for(i = n; i >= 0; i--)
	{
		queue_finalize(Q[i]);
	}
	free(event->customer);
	event->customer = NULL;
	free(event);
	event = NULL;
	priority_finalize(Events);

	return 0;
}
