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
#include "../lib/socket_helper.h" /* Provides sh_* funcs */


/****************************************
        Author: Tim Wood
        with a little help from
        http://beej.us/guide/bgnet/
****************************************/

/* Handle a request from a client */
void handle_request(int clientfd){
        int bytes_read;
        char message[256];

        bytes_read = read(clientfd, message, 256);

        if(bytes_read < 0) {
                perror("ERROR reading socket");
        }
        else if (bytes_read == 0){
                printf("Client disconnected");
        }
        else {
                printf("Client says %s\n", message);
        }
}


/* Main server logic */
void server_main(int sockfd) {

        while(1) {
                struct sockaddr_storage client_addr;
                socklen_t addr_size;
                int clientfd;

                addr_size = sizeof client_addr;
                clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
                sh_print_client_ip(client_addr);

                handle_request(clientfd);

                close(clientfd);
        }
}

int main(int argc, char ** argv)
{
        char* server_port = "1234";
        int sockfd;
        int o;

        /* Command line args:
                -p port
        */
        while ((o = getopt (argc, argv, "p:")) != -1) {
                switch(o){
                case 'p':
                        server_port = optarg;
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

        printf("listening on port: %s\n", server_port);

        sockfd = sh_server(server_port);

	/* Loop forever accepting new connections. */
        server_main(sockfd);

        out:
        close(sockfd);

        printf("Done.\n");
        return 0;
}
