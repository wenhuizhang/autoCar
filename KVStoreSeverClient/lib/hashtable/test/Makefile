CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -fprofile-arcs -ftest-coverage
DEPS = ../hashtable.o ../../linkedlist/linkedlist.o
OBJ = test-hashtable

all: hashtable_test

.PHONY: test

rebuild: clean all

hashtable_test:
	$(CC) $(CFLAGS) test-hashtable.c -o $(OBJ) $(DEPS)

test:
	./test-hashtable

clean:
	rm -f $(OBJ)
