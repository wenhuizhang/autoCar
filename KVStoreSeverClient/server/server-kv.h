/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Tim Wood, Eric Armbrust,
 *    Neel Shah, Wei Zhang, Chenghu He,
 *    Wenhui Zhang
 *
 * This program is licensed under the MIT license.
 *
 * server_kv.h - Declares the functions for
 *    server_kv.c and declares the hash table
 *************************************************/

#define MAX_CONCURRENCY 1000

struct pool_list {
	int fd;
	struct pool_list *next;
};

// This is the hashtable on the server
struct gwkv_server *ht;

void *handle_request(void *ptr);
void server_main(int sockfd, char* thread_number);

