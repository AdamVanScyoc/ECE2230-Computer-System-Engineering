//#include "sim.h"
#include "customer.h"
#include "event.h"
#include "queue.h"
#include "list.h"

#include <stdio.h>
// #include <string.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
	customer_t c1 =
	{
		.arrival_time = 1.0,
		.enqueue_time = 2.0,
		.checkout_time = 3.0,
		.leave_time = 4.0
	};

	customer_t c2 =
	{
		.arrival_time = 5.0,
		.enqueue_time = 6.0,
		.checkout_time = 7.0,
		.leave_time = 7.0
	};

	customer_t c3 =
	{
		.arrival_time = 2.0,
		.enqueue_time = 6.0,
		.checkout_time = 3.0,
		.leave_time = 1.0
	};

	customer_t c4 =
	{
		.arrival_time = 8.0,
		.enqueue_time = 2.0,
		.checkout_time = 1.0,
		.leave_time = 9.0
	};

	queue_t *q = queue_init(4);
	assert(q != NULL);
	assert(queue_remove(q) == NULL);
	assert(queue_size(q) == 0);
	assert(queue_insert(q, &c4) == 0);
	assert(queue_insert(q, &c2) == 0);
	assert(queue_insert(q, &c1) == 0);
	assert(queue_size(q) == 3);
	assert(queue_insert(q, &c3) == 0);

	assert(queue_full(q) == 1);
	assert(queue_peek(q) == &c4);

	// Order should the same as when you added
	assert(queue_remove(q) == &c4);
	assert(queue_remove(q) == &c2);
	assert(queue_remove(q) == &c1);
	assert(queue_remove(q) == &c3);


	queue_finalize(q);

	return 0;
}

// /* create and initialize a new queue
//  must be able to hold at least size items
//  return pointer to the new queue, NULL if error */
// queue_t *queue_init(int size);

// /* insert an item into the queue
//  return 0 if successful, -1 otherwise */
// int queue_insert(queue_t *q, customer_t *c);

// /* return the next item in the queue but do not remove it
//  return NULL if the queue is empty or on error */
// customer_t *queue_peek(queue_t *q);

//  remove the next item from the queue
//  and return it, return NULL if there is an error 
// customer_t *queue_remove(queue_t *q);

// /* return the number of items in the queue
//  You can see if queue is empty with this */
// int queue_size(queue_t *q);

// /* return nono-zero if the queue is full
//  This may be trivial using a linked implementation */
// int queue_full(queue_t *q);

// /* free all resourced used by the queue then free
//  the queue itself */
// void queue_finalize(queue_t *q);
