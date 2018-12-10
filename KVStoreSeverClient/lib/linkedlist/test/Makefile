#/************************************************
#*                     GW KV
#*  https://github.com/gwAdvNet2015/gw-kv-store
#*
#* Copyright 2015 Phil Lopreiato, Neel Shah,
#*    Eric Armbrust
#*
#* This program is licensed under the MIT license.
#*
#* Makefile - Compiles the linked list test file
#*************************************************/

CC = gcc
CFLAGS = -std=c99 -Wall -pedantic
DEPS = ../linkedlist.o
OBJ = test-linkedlist

.PHONY: test
all: linkedlist_test

rebuild: clean all

linkedlist_test:
	$(CC) $(CFLAGS) test-ll.c -o $(OBJ) $(DEPS)

test:
	./test-linkedlist

clean:
	rm -f $(OBJ)
