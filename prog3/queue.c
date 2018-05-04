/*-----------------------------*//*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 3
*/

/* queue.c */

//#define DEBUG 1

#include <stdio.h>
#include <stdlib.h>

#include "customer.h"
#include "event.h"
#include "queue.h"
#include "list.h"

// structure that describes a queue in entirety
struct queue_s 
{
	list list;	// the queue's 2-way linked list 
	int numnodes;	// number of nodes in the queue 
	int maxsize;	// maximum allowable size of queue 
	int queuenumber; // identifier 
};

/* create and initialize a new queue
must be able to hold at least size items
return pointer to the new queue, NULL if error */
queue_t *queue_init(int size)
{
	static int numqueues = 0; // tracks number of queues so we can assign each a UID

	if (size <= 0)
		return NULL;
	
	queue_t * newQueue = malloc(sizeof(*newQueue));
	if (!newQueue)
		return NULL;	

	newQueue->list = list_init();
	if (!newQueue->list)
	{
		// initialization of list failed, free queue and return error
		free(newQueue);
		return NULL;
	}
		
	newQueue->numnodes = 0;
	newQueue->maxsize = size;
	newQueue->queuenumber = ++numqueues;

	return newQueue;
}

/* insert an item into the queue
return 0 if successful, -1 otherwise */
int queue_insert(queue_t *q, customer_t *c)
{
	if (!q || !c)
	{
		#ifdef DEBUG
		printf("Error: queue_insert() passed a NULL pointer!\n");
		#endif
		return -1;
	}
	
	// check that queue is not already full
	if (queue_full(q))
	{
		#ifdef DEBUG	
		printf("Error: in queue_insert(); queue full!\n");
		#endif
		return -1;
	}

	//customer_t * newNode = list_insert(q->list, (list_data) c);
	customer_t * newNode = list_append(q->list, (list_data) c);
	if (!newNode)
	{
		#ifdef DEBUG
		printf("Error: in queue_insert(); failled to insert new node at head of list!\n");
		#endif
		return -1;
	}		
	
	q->numnodes++;

	return 0;
}

/* return the next item in the queue but do not remove it
return NULL if the queue is empty or on error */
customer_t *queue_peek(queue_t *q)
{
	if (!q)
	{
		#ifdef DEBUG
		printf("Error: queue_peek() passed NULL pointer!\n");
		#endif
		return NULL;
	}

	if (0 == queue_size(q) || !q->list)
	{
		#ifdef DEBUG
		printf("Error: queue_peek() passed a queue with no nodes!\n");
		#endif
		return NULL;
	}

	customer_t * newCust = (customer_t *)list_first(q->list);
	if (!newCust)
	{
		#ifdef DEBUG
		printf("Error: in queue_peek, call to list_first failed!\n");
		#endif
		return NULL;
	}
	
	return newCust;
}

/* remove the next item from the queue
and return it, return NULL if there is an error */
customer_t *queue_remove(queue_t *q)
{
	if (!q)
	{
		#ifdef DEBUG
		printf("Error: queue_remove() passed a NULL pointer!\n");
		#endif
		return NULL;
	}

	if (0 == queue_size(q) || !q->list)
	{
		#ifdef DEBUG
		printf("Error: queue_peek() passed a queue with no nodes!\n");
		#endif
		return NULL;
	}

	// retrieve head of linked list (and set cursor to it)
	customer_t * remCust = (customer_t *) list_first(q->list);
	if (!remCust)
	{
		#ifdef DEBUG
		printf("Error: in queue_remove(): failled to retrieve first node in queue!\n");
		#endif
		return NULL;
	}

	// cursor set to head of linked list; remove it and save the data stored therein
	remCust = (customer_t *) list_remove(q->list);
	if (!remCust)
	{
		#ifdef DEBUG
		printf("Error: in queue_remove(): failled to remove first node in queue!\n");
		#endif
		return NULL;
	}

	// decrement the number of nodes in queue
	q->numnodes--;

	return remCust;	
}

/* return the number of items in the queue
You can see if queue is empty with this */
int queue_size(queue_t *q)
{
	if (!q)
		return -1;

	return q->numnodes;
}

/* return nono-zero if the queue is full
This may be trivial using a linked implementation */
int queue_full(queue_t *q)
{
	if (!q)
		return -1;

	if (q->numnodes == (MAX_Q_SIZE <= q->maxsize ? MAX_Q_SIZE : q->maxsize))
		return 1;

	return 0;
}

/* free all resourced used by the queue then free
the queue itself */
void queue_finalize(queue_t *q)
{
	if (!q)
		return;

	if (q->list)
		list_finalize(q->list);

	free(q);	
}
