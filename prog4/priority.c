/* priority.c */
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 3
*/

// implemented as a binary tree stored in an array of fixed size

//#define DEBUG 1

#include <stdlib.h>
#include <stdio.h>

#include "customer.h"
#include "event.h"
#include "priority.h"

struct priority_s
{
	// the array that store the binary tree of events
	void * btree[MAX_E_SIZE + 1];
	// the number of nodes in the entire binary tree
	int numnodes;
	int maxsize;
};
		
/* create and initialize a new priority queue
must be able to hold at least size items
return pointer to the new priority queue, NULL if error */
priority_t * priority_init(int size)
{
	// return error if size is less than MAX_E_SIZE (because
	// otherwise, priority_full will cause a bug at some point
	if (size <= 0 || size > MAX_E_SIZE )
		return NULL;

	priority_t * newPriority = malloc(sizeof(*newPriority));
	if (!newPriority)
		return NULL;

	//newpriority queue contains 0 nodes
	newPriority->numnodes = 0;
	//save size for later use in priority_full()
	newPriority->maxsize = (size <= MAX_E_SIZE ? size: MAX_E_SIZE);

	return newPriority;
}

/* insert an item into the priority queue
return 0 if successful, -1 otherwise */
int priority_insert(priority_t *q, void * data, int priority)
{
	int childIndex = 0,
	    parentIndex = 0;

	if (!q)
		return -1;

	if (priority_full(q))
	{
		#ifdef DEBUG
		printf("When attempting to insert new event, event queue is full! Please re-run simulation with a larger event queue size.\n");
		#endif
		return -1;
	}

	// increment number of nodes, and do some sequential binary tree arithmetic to find
	// affected nodes
	q->numnodes++;
	childIndex = q->numnodes;
	parentIndex = childIndex/2;

	while (parentIndex)	// while there still is a higher priority node (one w/ a lesser
				// time
	{
		// the parent - that is, the node with the "higher" priority - 
		// is the one with the LESSER time
		if (ev->event_time > q->btree[parentIndex].event_time)
		{	// new event to be added has lesser priority - more recent time - 
			// and belongs here - as a child of current node

			q->btree[childIndex] = *ev;
			return 0;
		}

		else	
		{	// new event to be added has greater priority - and older time - 
			// and we need to traverse back up the tree toward the root to
			// find where to insert it

			// swap parent and child nodes as we traverse back up tree
			q->btree[childIndex] = q->btree[parentIndex];
			childIndex = parentIndex;
			parentIndex = parentIndex/2; // btree arithmetic to find next parent node

		}
	}

	// finally, we've reached position in tree where new node should be inserted
	q->btree[childIndex] = *ev;

	return 0; // success
}

/* remove the highest priority item from the queue
and return it, return NULL if there is an error */
event_t *priority_remove(priority_t *q)
{
	int curIndex,	// index of the current node/location
	 childIndex;	// index of child of curIndex
	event_t tempEvent; // event that will be relocated
	event_t * rtrnEvent; // event to be returned (the root of tree)

	rtrnEvent = malloc(sizeof(*rtrnEvent));
	if (!rtrnEvent)
		return NULL;

	if (!q)
	{
		#ifdef DEBUG
		printf("Error: priority_remove() passed a NULL pointer!\n");
		#endif
		free(rtrnEvent);
		return NULL;
	}
	else if (priority_empty(q))
	{
		#ifdef DEBUG
		printf("Error: priority_remove() called on a priority struct whose binary tree contains no nodes!\n");
		#endif
		free(rtrnEvent);
		return NULL;
	}
	
	*rtrnEvent = q->btree[1];
	tempEvent = q->btree[q->numnodes]; // set to last node (leaf) to be relocated later
	
	q->numnodes--; // delete last leaf in level order
	
	curIndex = 1; 	// initially, place current at root
	childIndex = 2*curIndex; // use sequential binary tree arithmetic to position us
		// at left child of curIndex

	while (childIndex <= q->numnodes)	// loop while a child still exists
	{	
		// of 2 children, set to the higher priority 
		// (higher priority = lesser[older] time
		if (childIndex < q->numnodes) // if this is the left child, 
						// and a right child exists 
		{
			// if right child is higher priority, set to right child
			if (q->btree[childIndex + 1].event_time < q->btree[childIndex].event_time)
				childIndex++;
		}

		// if item at childIndex > tempEvent, move it to curIndex, and move curIndex down
		if (q->btree[childIndex].event_time >= tempEvent.event_time)
		{
			q->btree[curIndex] = tempEvent;
			return (rtrnEvent);
		}	
		else
		{
			q->btree[curIndex] = q->btree[childIndex];
			curIndex = childIndex;
			childIndex = 2*curIndex;
		}
	}

	// we are positioned where tempEvent ultimately needs to be inserted
	q->btree[curIndex] = tempEvent;

	// return the original root of tree
	return (rtrnEvent);
		
}

/* return non-zero if the priority queue us empty */
int priority_empty(priority_t *q)
{
	if (!q)
		return 1;

	if (0 >= q->numnodes)
		return 1;

	return 0;
}

/* return nono-zero if the priority queue is full
This may be trivial using a linked implementation */
int priority_full(priority_t *q)
{
	if (!q)
		return 0;

	if (q->maxsize == q->numnodes)
		return 1;

	return 0;
}

/* free all resourced used by the priority queue then free
the queue itself */
void priority_finalize(priority_t *q)
{
	if (!q)
		return;
	
	// case where binary tree is empty
	if (q->numnodes == 0)
	{
		free(q);
		return;
	}	
	
	// note: the customer_t structs that the nodes of q contain are freed elsewhere

	free(q);
}
