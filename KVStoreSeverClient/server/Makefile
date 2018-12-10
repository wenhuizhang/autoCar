#/************************************************
#*                     GW KV
#*  https://github.com/gwAdvNet2015/gw-kv-store
#*
#* Copyright 2015 Phil Lopreiato, Neel Shah,
#*    Eric Armbrust, Chenghu He, Wenhui Zhang,
#*    Wei Zhang, Pradeep Kumar
#*
#* This program is licensed under the MIT license.
#*
#* Makefile - Compiles the hashtable wrapper
#*************************************************/

#Use the gcc compiler
CC = gcc
CFLAGS = -pthread
LDFLAGS =
TARGET = server-kv
DEPS = ../lib/linkedlist/linkedlist.o ../lib/hashtable/hashtable.o ../lib/murmurhash/murmurhash.o gwkv_ht_wrapper.o
OBJS = server-kv.o server-kv-run.o handle_operation.o ../lib/marshal/marshal.o
SRCS = server-kv.c server-kv-run.c handle_operation.c
WRAPPER_DEPS = gwkv_ht_wrapper.c
WRAPPER_OBJ = ht-wrapper murmurhash

#Ensure compiling is done with all necessary dependencies
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(TARGET)

rebuild: clean all

$(TARGET): ht-wrapper build-srcs
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(DEPS)

build-srcs:
	$(CC) $(CFLAGS) -c $(SRCS)

ht-wrapper: gwkv_ht_wrapper.c
	cd ../lib/ && make
	$(CC) $(CFLAGS) -c $(WRAPPER_DEPS)

test:
	cd test && make test

clean:
	@rm -f *.o $(OBJS)
.PHONY: all test
