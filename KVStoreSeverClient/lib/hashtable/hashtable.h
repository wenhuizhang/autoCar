/*
 * Copyright (c) 2014 Phil Lopreiato
 *
 * This program is licensed under the MIT license.
 * You should have recieved a copy of the full license terms
 * with this source code. If not, you can find it here: http://opensource.org/licenses/MIT
 */

#include "../linkedlist/linkedlist.h"

#ifndef FILE_HASHTABLE
#define FILE_HASHTABLE

#define ENABLE_REBALANCE 1
#define DISABLE_REBALANCE 0

/* Only nodes with unique keys are allowed to be added */
struct ht_node{
        char *key;
        void *value;
        int hash;
        unsigned short rebal;           // number of times this node has been rebalanced
};

/* Function pointer to calculate hash value for a node
 *
 * Parameters: char* that represents the key for the node in the table
 * Returns: an int that represents the hash value for the node,
 * however you want to calculate it
 */
#define HASH_FUNC int(*hash_func)(char* key)

/* Function pointer to a node comparison function
 *
 * Parameters: two ht_node* pointers that point to the nodes being compared
 * Returns an int depending on how the nodes compare to each other.
 * Ensure that two equal nodes produce a return value of 0
 */
#define NODE_EQUAL int(*node_equal)(struct ht_node* node1, struct ht_node* node2)

struct ht{
        unsigned short size;        // number of buckets in the list
        unsigned short used;        // number of buckets in use
        unsigned short current_max; // current largest bucket size
        unsigned short max_length;  // max size of a bucket list
        HASH_FUNC;                  // hash function we use
        NODE_EQUAL;                 // compare two nodes
        float fill_pct;             // max percent of bucket fillage
        struct ll **buckets;        // array of linkedlists where data is stored
        unsigned short rebal_count; // number of times the table has been rebalanced
        unsigned short in_rebalance;// flag for when we're rebalancing
        unsigned short allow_rebal; // let the table rebalance?
        unsigned short node_count;  // number of nodes in the table
};

/* Initialize a hashtable with all possible option */
struct ht*
ht_init(unsigned short size, unsigned short max_length, float fill_pct, unsigned short allow_rebal, HASH_FUNC, NODE_EQUAL);

/* Simple table init, do not allow rebalancing
 * Overrides the main init function
 */
#define ht_init_simple(size, hash_func, node_equal) ht_init(size, 0, 0, 0, hash_func, node_equal)

/* Allocate memory for a node
 * Will not populate the hash value yet
 * (That won't get populated until insert time
 */
struct ht_node*
ht_create_node(char* key, void* value);

/* Insert a node into the hashtable
 * This will not rebalance the table - that function
 * needs to be specifically called
 */
void
ht_insert(struct ht* table, struct ht_node* node);

/* Using the params we set during init, see if the
 * hashtable needs to be rebalanced
 */
unsigned short
ht_check_rebalance(struct ht* table);

/* Rebalances the table
 * Constructs a new table with twice as many buckets
 * and adds all the nodes into that one
 */
void
ht_rebalance(struct ht** table);

/* Look up a key in the table
 * returns NULL if not found
 */
struct ht_node*
ht_lookup(struct ht* table, char* key);

/* Lookup a predetermined hash value
 * returns NULL if not found
 */
struct ht_node*
ht_lookup_key(struct ht* table, int hash);

/* Get the bucket that the given hash maps to */
struct ll*
ht_get_bucket(struct ht* table, int hash);

void
ht_print(struct ht* table);

void
ht_print_node(struct ll_node* node);

void
ht_print_stats(struct ht* table);

/* free all memory associated with the hash table */
void
ht_free(struct ht* table);

void
ht_free_node(struct ll_node* node);

#endif
