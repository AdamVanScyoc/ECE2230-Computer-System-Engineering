test: test.o ../list.o ../priority.o ../queue.o 
	gcc test.o ../list.o ../priority.o ../queue.o -Wall -g -o test

test.o: test.c ../list.h ../priority.h ../queue.h ../event.h ../customer.h
	gcc test.c -c -Wall -g

priority.o : ../priority.h ../priority.c ../event.h ../customer.h
	gcc ../priority.c -c -Wall -g

queue.o : ../queue.h ../queue.c
	gcc ../queue.c -c -Wall -g

list.o : ../list.h ../list.c
	gcc ../list.c -c -Wall -g
