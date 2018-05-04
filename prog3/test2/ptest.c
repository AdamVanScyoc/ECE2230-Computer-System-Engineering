#include "customer.h"
#include "event.h"//"sim.h"
#include "priority.h"

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

	event_t ev1 =
	{
		.type = EV_ARRIVE,
		.event_time = 7.0,
		.customer = &c1
	};

	event_t ev2 =
	{
		.type = EV_ENQUEUE,
		.event_time = 90.0,
		.customer = &c1,
		.queue_number = 1
	};

	event_t ev3 =
	{
		.type = EV_CHECKOUT,
		.event_time = 30.0,
		.customer = &c1,
		.queue_number = 1
	};

	event_t ev4 =
	{
		.type = EV_LEAVE,
		.event_time = 45.0,
		.customer = &c1,
		.queue_number = 1
	};

	// event_t ev1cpy, ev2cpy, ev3cpy, ev4cpy;
	
	// memcpy(&ev1cpy, &ev1, sizeof(ev1cpy)),
	// memcpy(&ev2cpy, &ev2, sizeof(ev2cpy)),
	// memcpy(&ev3cpy, &ev3, sizeof(ev3cpy)),
	// memcpy(&ev4cpy, &ev4, sizeof(ev4cpy));

	priority_t *pq = priority_init(4);
	assert(pq != NULL);
	assert(priority_empty(pq) == 1);
	assert(priority_remove(pq) == NULL);

	assert(priority_insert(pq, &ev4) == 0);
	assert(priority_insert(pq, &ev2) == 0);
	assert(priority_insert(pq, &ev1) == 0);
	assert(priority_insert(pq, &ev3) == 0);
	assert(priority_full(pq) == 1);

	// Order should be ev1, ev3, ev4, ev2
/*
	assert(priority_remove(pq) == &ev1);
	assert(priority_remove(pq) == &ev3);
	assert(priority_remove(pq) == &ev4);
	assert(priority_remove(pq) == &ev2);
	assert(priority_empty(pq) == 1);
*/

	event_t * temp = priority_remove(pq);
	assert(temp->event_time == ev1.event_time);
	free(temp);
	temp = priority_remove(pq);
	assert(temp->event_time == ev3.event_time);
	free(temp);
	temp = priority_remove(pq);
	assert(temp->event_time == ev4.event_time);
	free(temp);
	temp = priority_remove(pq);
	assert(temp->event_time == ev2.event_time);
	assert(priority_empty(pq) == 1);
	free(temp);

	priority_finalize(pq);

	return 0;
}
