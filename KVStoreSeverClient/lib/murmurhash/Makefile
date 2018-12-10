#Use the gcc compiler
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall
#List of dependency files needed for compilation
DEPS = murmurhash.c
LDFLAGS =
OBJS = murmurhash test

all: $(OBJS)

rebuild: clean all

murmurhash:
	$(CC) $(CFLAGS) -c $(DEPS)

test:
	@echo "No tests for murmurhash"

clean:
	rm -f murmurhash.o
