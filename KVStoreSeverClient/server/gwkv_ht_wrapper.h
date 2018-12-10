/************************************************
*                     GW KV
*  https://github.com/gwAdvNet2015/gw-kv-store
*
* Copyright 2015 Phil Lopreiato, Stephen Monaghan, 
*   Jongmin Bae
*
* This program is licensed under the MIT license.
*
* gwkv_ht_wrapper.h - Declares the functions
*    for the gwkv_ht_wrapper.c file
*************************************************/

#ifndef GWKV_HT_WRAPPER
#define GWKV_HT_WRAPPER

#include <stdlib.h>
#include <pthread.h>
#include "../lib/marshal/marshal.h"
#include "../lib/hashtable/hashtable.h"
#include "../lib/murmurhash/murmurhash.h"

/* Types of hashing algorithms we support */
typedef enum {
       MURMUR
} hash_type;

/* All encompassing datatype for server datastore */
struct gwkv_server {
        struct ht* hashtable;
        hash_type hash;
        pthread_mutex_t lock;
};

/* Defines for hashtable params */
#define HT_SIZE 10 // number of buckets
#define HT_BUCKET_LENGTH 10 // max entry/bucket (only if rebalancing)
#define HT_FILL_PCT 0.1 // % bucket fill (only if rebalancing)
#define HT_REBAL 0 //disable rebalancing for now

/* Initialize a new key/value datastore */
struct gwkv_server*
gwkv_server_init(hash_type hash_algorithm);

/* Function to perform MD5 hash of key */
int
gwkv_murmur_hash(char* key);

/* Function to compare the equality of two entries */
int
gwkv_node_cmp(struct ht_node* node1, struct ht_node* node2);

/**
 * Wrapper function to set a value in the hashtable
 * Preconditions:
 *      - server has been initialized with gwkv_server_init
 *      - key and value are not NULL
 * Returns either STORED or NOT_STORED (defined above)
 */
int
gwkv_server_set (struct gwkv_server* server,
                 char *key,
                 size_t key_length,
                 char *value,
                 size_t value_length);

/**
 * Wrapper function to read a value from the hashtable
 * Returns the data if sucessful, or NULL on error
 * These correspond to the EXISTS and NOT_FOUND codes above
 * Preconditions:
 *      - server is initialized with gwkv_server_init
 *      - key is not NULL
 */
char*
gwkv_server_get (struct gwkv_server* server,
                 char *key,
                 size_t key_length,
                 size_t value_length);

/* Frees all memory associated with the datastore */
void
gwkv_server_free(struct gwkv_server* server);

#endif
