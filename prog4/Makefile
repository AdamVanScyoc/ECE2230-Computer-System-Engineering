prog4 : prog4.o list.o novel.o hash.o fileio.o
	gcc prog4.o list.o novel.o hash.o fileio.o -Wall -g -o prog4

prog4.o: prog4.c list.h list.c novel.h novel.c hash.h hash.c
	gcc prog4.c -c -Wall -g

novel.o : novel.h novel.c
	gcc novel.c -c -Wall -g

hash.o : hash.h hash.c
	gcc hash.c -c -Wall -g

list.o : list.h list.c
	gcc list.c -c -Wall -g

fileio.o : fileio.h fileio.c
	gcc fileio.c -c -Wall -g
