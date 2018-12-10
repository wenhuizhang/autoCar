#/************************************************
#*                     GW KV
#*  https://github.com/gwAdvNet2015/gw-kv-store
#*
#* Copyright 2015 Stephen Monaghan, Jongmin Bae
#*
#* This program is licensed under the MIT license.
#*
#* Makefile - Compiles the timer utility test
#*************************************************/

OBJS=test.o
CFLAGS= -I. -w
#DEFINES=-DTHINK_TIME
DEPS = timer_util.h
CC = gcc
BIN = test

all:
	make test.o
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) -lm

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJS)
	rm $(BIN)
