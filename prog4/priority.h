/*-------------------------*/ 
/*
* Adam Van Scyoc (Avanscy@clemson.edu)
* ECE 2230 SEC 001
* FALL 2017
* ASSIGNMENT 3
*/

/* priority.h */
#ifndef PRIORITY_H_
#define PRIORITY_H_

#ifndef MAX_E_SIZE 

#define MAX_E_SIZE 999999 // max size/number of nodes of event (priority) queue

#endif /* MAX_E_SIZE */

// implemented as a binary tree stored in an array of fixed size (btree.h/c)
// the event queue

// opaque data type that describes an entire event/priority queue
typedef struct priority_s priority_t;

/* create and initialize a new priority queue
must be able to hold at least size items
return pointer to the new priority queue, NULL if error */
priority_t * priority_init(int size);

/* insert an item into the priority queue
return 0 if successful, -1 otherwise */
int priority_insert(priority_t *q, int priority);

/* remove the highest priority item from the queue
and return it, return NULL if there is an error */
event_t *priority_remove(priority_t *q);

/* return non-zero if the priority queue us empty */
int priority_empty(priority_t *q);

/* return nono-zero if the priority queue is full
This may be trivial using a linked implementation */
int priority_full(priority_t *q);

/* free all resourced used by the priority queue then free
the queue itself */
void priority_finalize(priority_t *q);

#endif /* PRIORITY_H_ */
/*-------------------------------*/
