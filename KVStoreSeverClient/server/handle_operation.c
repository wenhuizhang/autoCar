/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Neel Shah
 *
 * This program is licensed under the MIT license.
 *
 * handle_operation.c - Determines GET vs SET
 *    operations
 *************************************************/

#include "../lib/marshal/marshal.h"
#include "handle_operation.h"
#include "../common.h"

char*
gwkv_handle_operation(struct gwkv_server *ht, char *cmd)
{
        struct operation *op = malloc( sizeof(struct operation) );
        int status;
        char *ht_set, *ht_get;

        status = gwkv_demarshal_server(cmd, &op);
        if (status == -1) {
                perror("Error on demarshal server");
                exit(-1);
        }

        switch(op->method_type) {
        case GET:
                kvprintf("[!] inside gwkv_handle_operation | entered switch | inside case GET\n");
                ht_get = gwkv_handle_get(ht, op);
                if (ht_get == NULL) {
                        perror("Something failed in gwkv_handle_get");
                        exit(-1);
                }
                kvprintf("[!] inside gwkv_handle_operation | entered switch | case GET done -- here's the message %s\n", ht_get);
                return ht_get;
        case SET:
                kvprintf("[!] inside gwkv_handle_operation | entered switch | inside case SET\n");
                ht_set = gwkv_handle_set(ht, op);
                if (ht_set == NULL) {
                        perror("Something failed in gwkv_handle_set");
                        exit(-1);
                }
                kvprintf("[!] inside gwkv_handle_operation | entered switch | case SET done | here's the message -- %s\n", ht_set);
                return ht_set;
        default:
                perror("Wrong command, switch dying");
                exit(-1);
        }

        return NULL;
}

char*
gwkv_handle_get(struct gwkv_server *ht, struct operation *op)
{
        char *ht_get;   // returns the value from the hashtable
        char *msg;      // this is the message that gwkv_marshal_server will craft

        ht_get = gwkv_server_get(ht, op->key, op->key_length, op->value_length);
        if (ht_get == NULL) {
                kvprintf("[!] inside gwkv_handle_get | ht_get == NULL | crafting message with NOT_FOUND\n");
                gwkv_craft_message(op, NOT_FOUND, &msg);
        } else {
                kvprintf("[!] inside gwkv_handle_get | ht_get != NULL | crafting message with EXISTS\n");
                op->value = ht_get;
                gwkv_craft_message(op, EXISTS, &msg);
        }

        kvprintf("[!] inside gwkv_handle_get | here is the crafted GET message -- %s\n", msg);

        return msg;
}

char*
gwkv_handle_set(struct gwkv_server *ht, struct operation *op)
{
        int ht_set;     // returns STORED or NOT_STORED from hashtable
        char *msg;      // this is the message that gwkv_marshal_server will craft

        ht_set = gwkv_server_set(ht, op->key, op->key_length, op->value, op->value_length);

        kvprintf("[!] inside gwkv_handle_set | result of gwkv_server_set = %d\n", ht_set);

        switch(ht_set) {
        case STORED:
                kvprintf("[!] inside gwkv_handle_set | entered switch | case STORED | crafting message now\n");
                gwkv_craft_message(op, STORED, &msg);
                break;
        case NOT_STORED:
                kvprintf("[!] inside gwkv_handle_set | entered switch | case NOT_STORED | crafting message now\n");
                gwkv_craft_message(op, NOT_STORED, &msg);
                break;
        default:
                perror("gwkv_server_set failed, dying");
                exit(-1);
        }

        kvprintf("[!] inside gwkv_handle_set | here\'s the crafted message for set command -- %s\n", msg);

        return msg;
}

int
gwkv_craft_message(struct operation *op, int status, char **msg)
{
        int res;

        kvprintf("[!] inside gwkv_craft_message | about to call gwkv_marshal_server to craft message\n");

        // Set lengths, because counting
        op->key_length = strlen(op->key);
	if (op->value != NULL ) {
                op->value_length = strlen(op->value);
        }

        res = gwkv_marshal_server(op, status, msg);

        kvprintf("[!] inside gwkv_craft_message | here\'s the crafted message -- %s\n", res);

        if (res != 0) {
                return -1;
        } else {
                return 0;
        }
}

