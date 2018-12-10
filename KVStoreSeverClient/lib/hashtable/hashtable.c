/*
 * Copyright (c) 2014 Phil Lopreiato
 *
 * This program is licensed under the MIT license.
 * You should have recieved a copy of the full license terms
 * with this source code. If not, you can find it here: http://opensource.org/licenses/MIT
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#include "../linkedlist/linkedlist.h"

/* Initialize a hash table.
 * Takes arguments for num buckets, max elements/bucket,
 * Max % of buckets to be filled, function for hashing nodes,
 * and function for equality comparison
 */
struct ht*
ht_init(unsigned short size, unsigned short max_length, float fill_pct, unsigned short allow_rebal, HASH_FUNC, NODE_EQUAL)
{
        struct ht *table = malloc(sizeof(struct ht));
        table->size = size;
        table->used = 0;
        table->max_length = max_length;
        table->fill_pct = fill_pct;
        table->buckets = calloc(size, sizeof(struct ll));
        table->rebal_count = 0;
        table->in_rebalance = 0;
        table->allow_rebal = allow_rebal;
        table->hash_func = hash_func;
        table->node_equal = node_equal;
        table->current_max = 0;
        table->node_count = 0;
        for(int i=0; i<size; i++){
                table->buckets[i] = ll_init();
        }
        return table;
}

/* Create a node that's ready to be inserted into the hashtable */
struct ht_node*
ht_create_node(char *key, void *value)
{
        if(!key || !value) return NULL;
        struct ht_node *node = malloc(sizeof(struct ht_node));
        node->key = key;
        node->value = value;
        node->rebal = 0;
        node->hash = -1;
        return node;
}

/* Insert a node into the hashtable if it doesn't already exist */
void
ht_insert(struct ht *table, struct ht_node *node)
{
        struct ll *list;
        struct ht_node *lookup;

        if(!node || !table || table->size == 0) return;

        /* First, calculate the hashcode for the node */
        node->hash = (*table->hash_func)(node->key);

        /* Keep this node's rebalance count in sync with the table's */
        node->rebal = table->rebal_count;

        list = ht_get_bucket(table, node->hash);
        lookup = ht_lookup(table, node->key);

        if(!lookup || table->in_rebalance){
                /* If the node doesn't already exist in the table, insert it into the proper list
                 * Or, if we're building a new table while rebalancing
                 */
                struct ll_node *insert = ll_create_node(node);
                ll_append(list, insert);

                if(list->size == 1){
                        // if we're adding on a list with no prior elements
                        table->used++;
                }

                if(list->size > table->current_max){
                        // update the max list size, if this one's the largest
                        table->current_max = list->size;
                }

                table->node_count++;
        }
}

unsigned short
ht_check_rebalance(struct ht *table)
{
        // we want to rebalance our table if:
        // A list is over the maximum allowed length OR
        // most than the specified buckets are occupied

        unsigned short allowed_buckets = (unsigned short)(table->fill_pct * table->size);
        return table->allow_rebal &&
                                !table->in_rebalance &&
                                (table->used > allowed_buckets ||
                                 table->current_max > table->max_length);
}

/* If we need to, enlarge the size of the table so we can reduce the length of the lists */
void
ht_rebalance(struct ht **table)
{
        /* Do a 2x size increase as many times as we need to make the rebalance check pass */
        while(ht_check_rebalance(*table)){
                struct ll **buckets = (*table)->buckets;
                int oldsize = (*table)->size;
                struct ht *oldtable = *table;
                *table = ht_init(oldsize*2, (*table)->max_length, (*table)->fill_pct, (*table)->allow_rebal, (*table)->hash_func, (*table)->node_equal);
                free(oldtable);

                /* Move all nodes from the old table to the new table */
                (*table)->in_rebalance = 1;
                for(int i=0; i<oldsize; i++){
                        struct ll *bucket = buckets[i];
                        struct ll_node *node = bucket->head;
                        while(node){
                                struct ht_node *data = (struct ht_node*)node->data;
                                struct ll_node *temp = node->next;
                                free(node);
                                ht_insert(*table, data);
                                node = temp;
                        }
                }
                free(buckets);
                (*table)->in_rebalance = 0;
        }
}

/* Check and see if a node with a given key exists in the hashtable */
struct ht_node*
ht_lookup(struct ht *table, char* key)
{
        int hash = (*table->hash_func)(key);
        struct ll *list = ht_get_bucket(table, hash);
        struct ll_node *check_node = list->head;
        while(check_node){
                struct ht_node *data = (struct ht_node*)check_node->data;
                if(data->hash == hash){
                        return data;
                }
                check_node = check_node->next;
        }
        /* Node doesn't exist :( */
        return NULL;
}

/* Get a list of all the nodes that have a certain key (regardless of value) */
struct ht_node*
ht_lookup_key(struct ht* table, int hash)
{
        struct ll *list = ht_get_bucket(table, hash);
        struct ll_node *check_node = list->head;
        while(check_node){
                struct ht_node *data = (struct ht_node*)check_node->data;
                /* Since hash is based on key, we can compare those */
                if(data->hash == hash){
                        return data;
                }
                check_node = check_node->next;
        }
        /* Node doesn't exist, return NULL */
        return NULL;
}

/* Take the node's hashcode, mod with bucket count to get the index */
struct ll*
ht_get_bucket(struct ht *table, int hash_code)
{
        int bucket = hash_code % table->size;
        return table->buckets[bucket];
}

/* Print out the hash table! */
void
ht_print(struct ht *table)
{
        for(int i=0; i<table->size; i++){
                struct ll *bucket = table->buckets[i];
                ll_print(bucket, &ht_print_node);
        }
}

/* Function to show some states about the hashtable */
void
ht_print_stats(struct ht *table)
{
        unsigned short allowed_buckets = (unsigned short)(table->fill_pct * table->size);
        printf("Hashtable size: %u\n", table->size);
        printf("Autobalancing enabled? %s\n", table->allow_rebal?"Yes":"No");
        printf("Buckets used: %u\n", table->used);
        printf("Number of nodes: %u\n", table->node_count);
        printf("Largest bucket: %u\n", table->current_max);
        printf("Max bucket size: %u\n", table->max_length);
        printf("Max number of used buckets: %u\n",allowed_buckets);
}

/* Function for printing the nodes we've inserted into the linkedlists */
void
ht_print_node(struct ll_node *node)
{
        struct ht_node *data = (struct ht_node*)(node->data);
        char *value = (char*)data->value;
        printf("[(%s, %s): %d]", data->key, value, data->hash);
}

/* Free a hashnode */
void
ht_free_node(struct ll_node *node)
{
        struct ht_node *data = node->data;
        free(data);
}

/* Free the hashtable */
void
ht_free(struct ht *table)
{
        for(int i=0; i<table->size; i++){
           ll_free(table->buckets[i], &ht_free_node);
        }
        free(table->buckets);
        free(table);
}
