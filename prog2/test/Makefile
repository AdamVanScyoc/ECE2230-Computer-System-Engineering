test: test.o hack_inventory.o hack_object.o list.o
	gcc test.o hack_inventory.o hack_object.o list.o -o test -Wall -g

test.o : test.c list.h hack_inventory.h hack_object.h
	gcc test.c -c -Wall -g

hack_inventory.o : ../hack_inventory.c list.h hack_object.h hack_inventory.h
	gcc ../hack_inventory.c -c -Wall -g

hack_object.o : hack_object.h ../hack_object.c
	gcc ../hack_object.c -c -Wall -g

list.o : ../list.c list.h
	gcc ../list.c -c -Wall -g

