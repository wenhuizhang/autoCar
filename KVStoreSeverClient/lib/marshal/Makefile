#/************************************************
#*                     GW KV
#*  https://github.com/gwAdvNet2015/gw-kv-store
#*
#* Copyright 2015 Pradeep Kumar, Yang Hu, Grace Liu
#*
#* This program is licensed under the MIT license.
#*
#* Makefile - Compiles marshal files
#*************************************************/

#Use the gcc compiler
CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -g
#List of dependency files needed for compilation
DEPS = marshal.c
LDFLAGS =
OBJS = marshal tests

.PHONY: test
all: $(OBJS)

rebuild: clean all

marshal:
	$(CC) $(CFLAGS) -c $(DEPS)

tests:
	cd test && make

test:
	cd test && make test

clean:
	rm -f marshal.o
	cd test && make clean
