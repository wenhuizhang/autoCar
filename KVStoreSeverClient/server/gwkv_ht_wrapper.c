/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Phil Lopreiato
 *
 * This program is licensed under the MIT license.
 *
 * gwkv_ht_wrapper.c - Implements wrappers for
 *    the hash table using murmur hash
 *************************************************/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/murmurhash/murmurhash.h"
#include "../lib/hashtable/hashtable.h"
#include "gwkv_ht_wrapper.h"

struct gwkv_server*
gwkv_server_init(hash_type hash_algorithm)
{
        /* First, allocate memory */
        struct gwkv_server *server = malloc(sizeof(struct gwkv_server));
        if(!server){
                return NULL;
        }

        /* Set the hash function according to input */
        HASH_FUNC;
        server->hash = hash_algorithm;
        switch(hash_algorithm){
                /* Currently, only one type of hashing */
                default:
                case MURMUR:
                        hash_func = &gwkv_murmur_hash;
                        break;
        }

        /* Create a mutex for thread-safety */
        if(pthread_mutex_init(&server->lock, NULL)){
                /* Failed to create mutex. Free everything and die :( */
                free(server);
                return NULL;
        }

        /* Create the internal hashtable */
        server->hashtable = ht_init(HT_SIZE, HT_BUCKET_LENGTH, HT_FILL_PCT, HT_REBAL, hash_func, &gwkv_node_cmp);
        if(!server->hashtable){
                /* Failure malloc-ing hashtable memory. Die here */
                pthread_mutex_destroy(&server->lock);
                free(server);
                return NULL;
        }

        return server;
}

int
gwkv_server_set(struct gwkv_server* server,
                char* key,
                size_t key_length,
                char* value,
                size_t value_length)
{
        /* Variable defines */
        int expected_size;
        struct ht_node* existing_node;

        /* Precondition checks */
        if(!server || !server->hashtable || !key || !value){
                return NOT_STORED;
        }

        expected_size = server->hashtable->node_count;
        existing_node = ht_lookup(server->hashtable, key);
        pthread_mutex_lock(&server->lock);
        if(existing_node){
                /* Node already exists in the table, update its value */
                existing_node->value = value;
        }else{
                /* Node does not exist, add it */
                existing_node = ht_create_node(key, value);
                ht_insert(server->hashtable, existing_node);
                expected_size++;
        }

        pthread_mutex_unlock(&server->lock);

        /* Now, sanity check to ensure the node was actually entered */
        if(existing_node && server->hashtable->node_count == expected_size){
                return STORED;
        }else{
                return NOT_STORED;
        }
}

char*
gwkv_server_get(struct gwkv_server* server,
                char* key,
                size_t key_length,
                size_t value_length)
{
        struct ht_node* node;

        /* Precondition checks */
        if(!server || !server->hashtable || !key){
                return NULL;
        }

        node = ht_lookup(server->hashtable, key);
        if(!node){
                /* Node does not exist in table */
                return NULL;
        }

        void* data = node->value;
        if(!data){
                return NULL;
        }

        /* We only care about storing char* so we can cast here */
        return (char*)data;
}

void
gwkv_server_free(struct gwkv_server* server)
{
        pthread_mutex_destroy(&server->lock);
        ht_free(server->hashtable);
        free(server);
}

int
gwkv_murmur_hash(char* key){
	int hash = murmurhash(key, strlen(key), 0); //takes in the key, the length of the key, and a seed
        if(hash < 0) hash *= -1; // make sure we only use positive numbers
        return hash;
}

int
gwkv_node_cmp(struct ht_node* node1, struct ht_node* node2)
{
	int keycmp = strcmp(node1->key, node2->key);
	int valcmp = strcmp(node1->value, node2->value);
	if(keycmp == 0 && valcmp == 0){ return 0; }
	return 1;
}
