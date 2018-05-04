/*----------------------------*/
/* event.h */
#ifndef EVENT_H_
#define EVENT_H_

#define EV_ARRIVE 1
#define EV_ENQUEUE 2
#define EV_CHECKOUT 3
#define EV_LEAVE 4

// defines event structure

// the event structure itself
typedef struct event_s
{
	int type;
	double event_time;
	customer_t *customer;
	int queue_number;
} event_t;

#endif /* EVENT_H_ */
/*-------------------------------*/
