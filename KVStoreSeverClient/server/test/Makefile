CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -g
DEPS = ../../lib/linkedlist/linkedlist.o ../../lib/hashtable/hashtable.o ../../lib/murmurhash/murmurhash.o ../../lib/marshal/marshal.o ../gwkv_ht_wrapper.o ../handle_operation.o
OBJ = test-ht-wrapper

.PHONY: all test

all: ht_wrapper_test

rebuild: clean all

ht_wrapper_test:
	$(CC) $(CFLAGS) test_ht_wrapper.c -o $(OBJ) $(DEPS)

test: ht_wrapper_test
	./test-ht-wrapper

clean:
	rm -f $(OBJ)
