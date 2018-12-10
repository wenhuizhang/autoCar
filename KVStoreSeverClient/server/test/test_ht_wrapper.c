/* File to test hashtable wrapper */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gwkv_ht_wrapper.h"
#include "../../lib/hashtable/hashtable.h"

int
main(){
        struct gwkv_server* server;
        char* data;
        int result;

        printf("Testing hashtable wrapper\n");
        printf("Allocating server memory...");
        server = gwkv_server_init(MURMUR);
        assert(server);
        printf("OK\n");

        printf("Testing getting a nonexistent value...");
        data = gwkv_server_get(server, "foobar", 0, 0);
        assert(data == NULL);
        printf("OK\n");

        printf("Testing inserting a value...");
        result = gwkv_server_set(server, "foobar", 0, "meow", 0);
        assert(result == STORED);
        printf("OK\n");

        printf("Testing getting a value...");
        data = gwkv_server_get(server, "foobar", 0, 0);
        assert(strcmp(data, "meow") == 0);
        printf("OK\n");

        printf("All tests passed\n");
}
