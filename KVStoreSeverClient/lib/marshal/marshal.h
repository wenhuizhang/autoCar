/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Pradeep Kumar, Yang Hu, Grace Liu
 *
 * This program is licensed under the MIT license.
 *
 * marshal.h - defines the functions and structs
 *    for marshal.c
 *************************************************/

#ifndef __MARSHAL_H__
#define __MARSHAL_H__

#include <stdlib.h>

/* Defines for returned status */
#define STORED          0
#define NOT_STORED      1
#define EXISTS          2
#define NOT_FOUND       3

/* Support KV Store opperations */
typedef enum method {
        GET,
        SET
}method;

/* Operation struct used for gwkv opperations */
struct operation {
        method method_type;
        char* key;
        size_t key_length;
        char* value;
        size_t value_length;
};

int
gwkv_marshal_server(struct operation* data, int status, char** ascii);

int
gwkv_marshal_client(struct operation* data, char** ascii);

int
gwkv_demarshal_server(char* ascii, struct operation** op);

int
gwkv_demarshal_client(char* ascii, struct operation** op, int* status);
#endif//__MARSHAL_H__
