#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../marshal.h"
#define MAX_KEY_SIZE 250
#define MAX_VALUE_SIZE 1024	
struct operation *create_request(method op, char *key, char *value);

void
print_request(struct operation *op)
{
        printf("Method: %d\n", op->method_type);
        printf("Key: %s\n", op->key);
        printf("key_length: %d\n", op->key_length);
        printf("Value: %s\n", op->value);
        printf("value_length: %d\n\n", op->value_length);
}


struct operation *
create_request(method op, char *key, char *value)
{
        struct operation *query;
        query = malloc(sizeof(struct operation));
        query->method_type = op;
        query->key = key;
        query->key_length = strlen(query->key);
        query->value = value;
        query->value_length = 0;
        if (NULL != value) {
            query->value_length = strlen(query->value);
        }
        return query;
}

struct operation *
create_response(method op, char *key, char *value)
{
        struct operation *query;
        query = malloc(sizeof(struct operation));
        query->method_type = op;
        query->key = key;
        query->key_length = strlen(query->key);
        query->value = value;
        query->value_length = 0;
        if (NULL != value) {
            query->value_length = strlen(query->value);
        }
        return query;
}


int
cmp_request(struct operation *op1, struct operation *op2)
{
        if (op1->method_type == op2->method_type &&
            strcmp(op1->key, op2->key) == 0 &&
            op1->key_length == op2->key_length &&
            (op1->value != NULL ? strcmp(op1->value, op2->value) : 0)  == 0 &&
            op1->value_length == op2->value_length
           )
                return 0;
        else
                return -1;
}

int
test_demarshal_get(char *get_str, struct operation *get_request)
{
        struct operation *get_op;
        int get_status;
        assert(gwkv_demarshal_client(get_str, &get_op, &get_status) == 0);
        //int ret = gwkv_demarshal_client(get_str, &get_op, &get_status);
        //print_request(get_op);
        //assert(ret == 0);
        assert(cmp_request(get_op, get_request) == 0);
        return 0;
}

int
test_demarshal_set(int status)
{
        struct operation *set_op;
        int set_status;
        char *stored = "STORED\r\n";
        char *not_stored = "NOT_STORED\r\n";
        char *exits = "EXISTS\r\n";
        char *not_found = "NOT_FOUND\r\n";

        switch(status) {
                case STORED:
                        assert(gwkv_demarshal_client(stored, &set_op, &set_status) == 0);
                        assert(set_status == STORED);
                        break;
                case NOT_STORED:
                        assert(gwkv_demarshal_client(not_stored, &set_op, &set_status) == 0);
                        assert(set_status == NOT_STORED);
                        break;
                case EXISTS:
                        assert(gwkv_demarshal_client(exits, &set_op, &set_status) == 0);
                        assert(set_status == EXISTS);
                        break;
                case NOT_FOUND:
                        assert(gwkv_demarshal_client(not_found, &set_op, &set_status) == 0);
                        assert(set_status == NOT_FOUND);
                        break;
        }
        return 0;
}



int
main()
{
        //used as verifier and an input parameter. Not to be modified.
        struct operation *get_request, *set_request, *get_response;//set_response is status;
        
        //used as ooutput parameter for apis. 
        struct operation *get_op, *set_op;
        char *ascii;
        char *key = "test_key";
        char *value = "test_value";
        char *set_cmd = "set test_key 0 0 10\r\ntest_value\r\n";
        char *get_cmd = "get test_key\r\n";
        char *get_str = "VALUE test_key 0 10\r\ntest_value\r\nEND\r\n";
        int status = 0;
        
        printf("Create get and set request with key %s and value %s\n", key, value);
        get_request = create_request(GET, key, NULL);
        set_request = create_request(SET, key, value);
        get_response = create_response(GET, key, value);


        printf("Test gwkv_marshal_server...\n");
        assert(gwkv_marshal_server(get_response, status, &ascii) == 0);
        printf("- Get Result:\n%s\n", ascii);
        assert(ascii != NULL);
        assert(strcmp(ascii, get_str) == 0);
        ascii = 0;
        free(ascii);
        printf("- PASS\n");
        assert(gwkv_marshal_server(set_request, status, &ascii) == 0);
        printf("- Set Result:\n%s\n", ascii);
        assert(ascii != NULL);
        assert(strcmp(ascii, set_cmd) == 0);
        ascii = 0;
        free(ascii);
        printf("- PASS\n");
        
        printf("Test gwkv_marshal_client...\n");
        assert(gwkv_marshal_client(get_request, &ascii) == 0);
        printf("- Get Result:\n%s\n", ascii);
        assert(ascii != NULL);
        assert(strcmp(ascii, get_cmd) == 0);
        ascii = 0;
        free(ascii);
        assert(gwkv_marshal_client(set_request, &ascii) == 0);
        printf("- PASS\n");
        printf("- Set Result:\n");
        assert(ascii != NULL);
        assert(strcmp(ascii, set_cmd) == 0);
        ascii = 0;
        free(ascii);
        printf("- PASS\n\n");

        printf("Test gwkv_demarshal_server...\n");
        gwkv_demarshal_server(get_cmd, &get_op);
        assert(cmp_request(get_op, get_request) == 0);
        printf("- Get Result: PASS\n");
        gwkv_demarshal_server(set_cmd, &set_op);
        assert(cmp_request(set_op, set_request) == 0);
        printf("- Set Result: PASS\n");

        printf("Test gwkv_demarshal_client...\n");
        assert(test_demarshal_get(get_str, get_response) == 0);
        printf("- Get Result: PASS\n");
        assert(test_demarshal_set(STORED) == 0);
        printf("- Set STORED: PASS\n");
        assert(test_demarshal_set(NOT_STORED) == 0);
        printf("- Set NOT_STORED: PASS\n");
        assert(test_demarshal_set(EXISTS) == 0);
        printf("- Set EXISTS: PASS\n");
        assert(test_demarshal_set(NOT_STORED) == 0);
        printf("- Set NOT_FOUND: PASS\n");
        printf("- Set Result: PASS\n");
        
        printf("All marshal tests passed\n\n");
        return 0;
}



