/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Tim Wood, Eric Armbrust,
 *     Wei Zhang, Wenhui Zhang, Neel Shah,
 *     Chenghu He
 *
 * With a little help from
 * http://beej.us/guide/bgnet/
 *
 * This program is licensed under the MIT license.
 *
 * server_kv.c - Starts the server and handles
 *    requests.
 *************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "server-kv.h"
#include "gwkv_ht_wrapper.h"
#include "handle_operation.h"
#include "../lib/hashtable/hashtable.h"


struct pool_list *list_head = NULL, *list_tail = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_t threads[MAX_CONCURRENCY];

/* Handle a request from a client */
void *
handle_request(void *ptr)
{
        int bytes_read, bytes_write;
        char message[256];
	struct operation operation;
	struct pool_list *node;
	int clientfd;

	while (1) {
		pthread_mutex_lock(&mutex);
		while (list_head == NULL) {
			pthread_cond_wait(&cond, &mutex);
		}
		node = list_head;
		list_head = node->next;
		pthread_mutex_unlock(&mutex);
                clientfd = node->fd;

                //for test
                bytes_read = read(clientfd, message, 256);
		if(bytes_read < 0) {
			perror("ERROR reading socket");
		}
		else if (bytes_read == 0){
			printf("Client disconnected");
		}
		else {
                        strcpy(message, gwkv_handle_operation(ht, message));
                        bytes_write = strlen(message)+1;
			write(clientfd, message, bytes_write);
                        printf("thread: %d, received:%s\n", pthread_self(), message);
		}
                //

                close(clientfd);
		free(node);
	}
}

/* Main server logic */
void
server_main(int sockfd, char* thread_number)
{
	int i, tnum;
	struct pool_list *node;

        ht = gwkv_server_init(MURMUR);

        tnum = atoi(thread_number);
	for(i = 0; i < tnum; i++) {
		pthread_create(&threads[i], NULL, handle_request, NULL);
	}

        while(1) {
                struct sockaddr_storage client_addr;
                socklen_t addr_size;
                int clientfd;

                addr_size = sizeof client_addr;
                clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
                //sh_print_client_ip(client_addr);
		node = (struct pool_list *)malloc(sizeof(struct pool_list));
		node->fd = clientfd;
		node->next = NULL;

		pthread_mutex_lock(&mutex);
		if (list_head == NULL) {
			list_head = node;
		}
		else {
			list_tail->next = node;
		}
		list_tail = node;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
        }
}

