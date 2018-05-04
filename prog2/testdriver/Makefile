CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g
# path to CUnit header files
CPPFLAGS = -I/users/nlmills/opt/cunit/include
# path to CUnit library
LDFLAGS = -L/users/nlmills/opt/cunit/lib
LDLIBS = -lcunit

assign2_test: hack_object.o list.o hack_inventory.o

hack_object.o: hack_object.h

list.o: list.h

hack_inventory.o: hack_inventory.h hack_object.h list.h

.PHONY: clean
clean:
	rm -f assign2_test hack_object.o list.o hack_inventory.o
