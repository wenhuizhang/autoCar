/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Neel Shah
 *
 * This program is licensed under the MIT license.
 *
 * handle_operation.h - Declares the functions
 *    for handle_operation.c
 *************************************************/

#ifndef __HANDLE_OP_H__
#define __HANDLE_OP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "./gwkv_ht_wrapper.h"
#include "../lib/marshal/marshal.h"

/*
 * handle_operation takes string received from socket
 * and processes it for the GET or SET command.
 *
 * @param char *msg - the command received from the socket
 *
 * @return char* - returns the crafted message
 */
char*
gwkv_handle_operation(struct gwkv_server *ht, char *cmd);

/*
 * handle_get gets the value from the server using the gwkv_server_get
 * function.  If the data is found, it will craft a message using
 * the marshall function and send it along its way.  If the message is not
 * found, it will send NOT_FOUND along its way.
 *
 * @param struct gwkv_server *ht - struct containing hashtable and other server info
 * @param struct operation *op - struct containing info about the gwkv operations
 *
 * @return char* - returns the crafted message
 */
char*
gwkv_handle_get(struct gwkv_server *ht, struct operation *op);

/*
 * handle_set takes the value out of the operation struct and then calls
 * gwkv_server_set to put it in the hashtable.  Then, it will craft a message
 * using gwkv_marshal_server and send it over the socket.
 *
 * @param struct gwkv_server *ht - struct containing hashtable and other server info
 * @param struct operation *op - struct containing info about the gwkv operations
 *
 * @return char* - returns the crafted message
 */
char*
gwkv_handle_set(struct gwkv_server *ht, struct operation *op);

/*
 * This function calls gwkv_marshal_server.  The point of it is a tampolene
 * to make setting up the variables easier.
 *
 * @param struct operation *op - the gwkv operations struct
 * @param int status - the status of the hashtable operation
 * @param char **msg - the place where the crafted message will be stored
 *
 * @return - status
 */
int
gwkv_craft_message(struct operation *op, int status, char **msg);

#endif
