avanscy.assign3: avanscy.assign3.o list.o priority.o queue.o randsim.o
	gcc avanscy.assign3.o list.o priority.o queue.o randsim.o -Wall -Ofast -o avanscy.assign3 -lm

avanscy.assign3.o: avanscy.assign3.c list.h priority.h queue.h event.h customer.h
	gcc avanscy.assign3.c -c -Wall -Ofast

priority.o : priority.h priority.c event.h customer.h
	gcc priority.c -c -Wall -Ofast

queue.o : queue.h queue.c
	gcc queue.c -c -Wall -Ofast

list.o : list.h list.c
	gcc list.c -c -Wall -Ofast

randsim.o : randsim.h randsim.c
	gcc randsim.c -c -Wall -lm -Ofast
