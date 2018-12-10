/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Phil Lopreiato, Neel Shah, 
 *   Eric Armbrust
 *
 * This program is licensed under the MIT license.
 *
 * linkedlist.h - Declares the functions for
 *    linkedlist.c
 *************************************************/

#ifndef FILE_LINKEDLIST
#define FILE_LINKEDLIST
struct ll_node{
        void *data;
        struct ll_node *next;
        struct ll_node *last;
};

struct ll{
        struct ll_node *head;
        struct ll_node *tail;
        int size;
};

//initialize empty list
struct ll *ll_init();

//create a node with the given value
struct ll_node *ll_create_node(void *value);

int ll_get_size();

//append node to the tail of the list
void ll_append(struct ll *list, struct ll_node *node);

//prepend node to the head of the list
void ll_prepend(struct ll *list, struct ll_node *node);

//return node from the front of the list
struct ll_node *ll_first(struct ll *list);

//return the node from the front of the list without removing it
struct ll_node *ll_first_peek(struct ll *list);

//return the node at the tail of the list
struct ll_node *ll_last(struct ll *list);

//peek at the tail node without removing it
struct ll_node *ll_last_peek(struct ll *list);

void ll_print(struct ll*, void(*print_node)(struct ll_node*));

// remove a node from the list
// DOES NOT FREE THE NODE
void ll_remove(struct ll *list, struct ll_node *node);

//clear everything in the list
void ll_clear(struct ll *list, void(*free_node)(struct ll_node*));

//clear the list and free the given pointer
void ll_free(struct ll *list, void(*free_node)(struct ll_node*));
#endif
