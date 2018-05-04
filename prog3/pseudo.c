/* initialize queues */
/* malloc new event and customer */
/* now schedule arrive event at t=0 and put in event queue */
while(!event_empty(eq))
{
	new_ev = event_remove(eq);
	time_set(new_ev.time);
	switch (new_ev.event_type)
	{
		case (EV_ARRIVE) :
			new_ev.customer.arrive_time = time_get();
			shoptime = TIME_SHOP();
			schedule enqueue for this customer
			if (MAX_CUST > num_customers++)
			{
				/* malloc new event and customer */
				next_customer = TIME_ARRIVE();
				schedule an arrive event
			}
		break;
		case (EV_ENQUEUE) :
			customer.enqueue_time = time_get();
			find the queue with the smallest line (for loop)
			if the queue is empty
			{
				schedule a checkout event for this queue t+0
			}
			insert customer onto queue
		break;
		case (EV_CHECKOUT) :
			customer.checkout_time = time_get();
			use queue_number to select a queue and remove a customer
			use TIME_CHECKOUT to find checkout time
			schedule a leave event for customer, queue_number
		break;
		case (EV_LEAVE) :
			customer.leave_time = time_get();
			if (queue is not empty)
			{
				schedule checkout event for this queue t+0
			}
			compute stats for customer
			free customer recordbreak;
		default :
		break;
	}
	free event if needed
}
Print overall stats
