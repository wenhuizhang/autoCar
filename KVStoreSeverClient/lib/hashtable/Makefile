#Use the gcc compiler
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g
#List of dependency files needed for compilation
DEPS = hashtable.c
LDFLAGS =
OBJS = hashtable tests

.PHONY: test
all: $(OBJS)

rebuild: clean all

hashtable:
	$(CC) $(CFLAGS) -c $(DEPS)

tests:
	cd test && make

test:
	cd test && make test

clean:
	rm -f hashtable.o
	cd test && make clean
