/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Tim Wood, Phil Lopreiato,
 *      Eric Armbrust, Neel Shah
 *
 * This program is licensed under the MIT license.
 *
 * A little help from:
 * http://beej.us/guide/bgnet/
 *
 * server_kv_run.c - Starts the server
 *************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "../common.h"
#include "../lib/socket_helper.h" /* Provides sh_* funcs */
#include "server-kv.h"
#include "gwkv_ht_wrapper.h"
#include "../lib/hashtable/hashtable.h"


int
main(int argc, char ** argv)
{
        char* server_port = "1234";
        char* thread_number = "2";
        int sockfd;
        int o;

        while ((o = getopt (argc, argv, "p:n:d:h")) != -1) {
                switch(o){
                case 'p':
                        server_port = optarg;
                        break;
                case 'n':
                        thread_number = optarg;
                        break;
                case 'd':
                        verbose_print = 1;
                        kvprintf("[!] verbose mode enabled\n");
                        break;
                case 'h':
                        printf("Command line args:\n"
                                        "\t -p port\n"
                                        "\t -n number of threads\n"
                                        "\t -d verbose output\n"
                                        "\t -h this menu\n");
                        break;
                case '?':
                        if(optopt == 'p') {
                                fprintf (stderr, "Option %c requires an argument.\n", optopt);
                        }
                        else {
                                fprintf (stderr, "Unknown argument: %c.\n", optopt);
                        }
                        break;
                }
        }

        printf("listening on port: %s, thread number %s\n", server_port, thread_number);

        sockfd = sh_server(server_port);

	/* Loop forever accepting new connections. */
        server_main(sockfd, thread_number);

        out:
        close(sockfd);

        printf("Done.\n");
        return 0;
}

