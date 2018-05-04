avanscy.assign3: avanscy.assign3.o list.o priority.o queue.o randsim.o
	gcc avanscy.assign3.o list.o priority.o queue.o randsim.o -Wall -g -o avanscy.assign3 -lm

avanscy.assign3.o: avanscy.assign3.c list.h priority.h queue.h event.h customer.h
	gcc avanscy.assign3.c -c -Wall -g

priority.o : priority.h priority.c event.h customer.h
	gcc priority.c -c -Wall -g

queue.o : queue.h queue.c
	gcc queue.c -c -Wall -g

list.o : list.h list.c
	gcc list.c -c -Wall -g

randsim.o : randsim.h randsim.c
	gcc randsim.c -c -Wall -lm -g
