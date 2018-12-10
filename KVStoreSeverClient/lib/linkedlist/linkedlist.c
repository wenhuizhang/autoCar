/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Phil Lopreiato, Neel Shah, 
 *   Eric Armbrust
 *
 * This program is licensed under the MIT license.
 *
 * linkedlist.c -Implements a linked list
 *************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"

struct ll*
ll_init()
{
        struct ll *list = malloc(sizeof(struct ll));

        if (!list) return NULL;

        list->head = NULL;
        list->tail = NULL;
        list->size = 0;

        return list;
}

struct ll_node*
ll_create_node(void *data)
{
        struct ll_node *node = malloc(sizeof(struct ll_node));

        if (!node) return NULL;

        node->data = data;
        node->next = NULL;
        node->last = NULL;

        return node;
}

int
ll_get_size(struct ll *list)
{
        return list->size;
}

void
ll_append(struct ll *list, struct ll_node *node)
{
        node->last = list->tail;
        if (node->last) node->last->next = node;

        node->next = NULL;
        list->tail = node;

        if (!list->head) list->head = node;

        list->size++;
}

void
ll_prepend(struct ll *list, struct ll_node *node)
{
        list->head->last = node;
        node->next = list->head;
        list->head = node;
        list->size++;
}

struct ll_node*
ll_first(struct ll *list)
{
        struct ll_node *node = list->head;

        if (node){
                list->head = node->next;
                list->head->last = NULL;
                list->size--;
        }

        return node;
}

struct ll_node*
ll_first_peek(struct ll *list)
{
        return list->head;
}

struct ll_node*
ll_last(struct ll *list)
{
        struct ll_node *node = list->tail;

        if (node){
                list->tail = node->last;
                list->tail->next = NULL;
                list->size--;
        }

        return node;
}

struct ll_node*
ll_last_peek(struct ll *list)
{
        return list->tail;
}

void
ll_print(struct ll *list, void(*print_node)(struct ll_node*))
{
        printf("List has %d elements\n", list->size);

        struct ll_node *node = list->head;

        while(node) {
                (*print_node)(node);
                printf("->");
                node = node->next;
        }

        printf("\n");
}

void
ll_remove(struct ll *list, struct ll_node *node)
{
        if (node->last) node->last->next = node->next;
        else list->head = node->next; // this node is the head of the list

        if (node->next) node->next->last = node->last;
        else list->tail = node->last; // this node is the tail

        list->size--;

        node->next = NULL;
        node->last = NULL;
}

void
ll_clear(struct ll *list, void(*free_node)(struct ll_node*))
{
        struct ll_node *temp;
        struct ll_node *node = list->head;

        while (node) {
                temp = node->next;
                if (free_node) (*free_node)(node->data);
                free(node);
                node = temp;
        }

        list->size = 0;
        list->head = NULL;
        list->tail = NULL;
}

void
ll_free(struct ll *list, void(*free_node)(struct ll_node*))
{
        ll_clear(list, free_node);
        free(list);
}
