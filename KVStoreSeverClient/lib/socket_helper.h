/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Tim Wood
 *
 * This program is licensed under the MIT license.
 *
 * socket_helper.h - A library to simplify setting
 *    up sockets.
 *************************************************/

#ifndef FILE_SOCKET_HELPER
#define FILE_SOCKET_HELPER

/****************************************
        Author: Tim Wood
        with a little help from
        http://beej.us/guide/bgnet/

   A library to simplify setting up sockets.

 ****************************************/

#define BACKLOG 10     // how many pending connections queue will hold


void sh_print_client_ip(struct sockaddr_storage client_addr){
        int ipindex;
        int ipaddr;

        printf("Client ip: ");
        for(ipindex=2;ipindex<6;ipindex++){
                ipaddr=(int)(((struct sockaddr *)&client_addr)->sa_data[ipindex]);
                ipaddr=(ipaddr+256)%256;
                printf("%d.",ipaddr);
        }
        printf("\n");
}

/* Used by clients to connect to a server. */
int sh_client(char* server_ip, char* server_port){
        int sockfd, rc;
        struct addrinfo hints, *server;

        /* The hints struct is used to specify what kind of server info we are looking for */
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM; /* or SOCK_DGRAM */

        /* getaddrinfo() gives us back a server address we can connect to.
           It actually gives us a linked list of addresses, but we'll just use the first.
         */
        if ((rc = getaddrinfo(server_ip, server_port, &hints, &server)) != 0) {
                perror(gai_strerror(rc));
                exit(-1);
        }

        /* Now we can create the socket and connect */
        sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
        if (sockfd == -1) {
                perror("ERROR opening socket");
                exit(-1);
        }
        rc = connect(sockfd, server->ai_addr, server->ai_addrlen);
        if (rc == -1) {
                perror("ERROR on connect");
                close(sockfd);
                exit(-1);
        }
        free(server);

        return sockfd;
}

/* Used by server to setup a listening socket. */

int sh_server(char* server_port) {
        int yes=1;
        struct addrinfo hints, *server;
        int sockfd, rc;

        /* The hints struct is used to specify what kind of server info we are looking for */
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM; /* or SOCK_DGRAM */
        hints.ai_flags = AI_PASSIVE;

        /* getaddrinfo() gives us back a server address we can connect to.
           The first parameter is NULL since we want an address on this host.
           It actually gives us a linked list of addresses, but we'll just use the first.
         */
        if (rc = getaddrinfo(NULL, server_port, &hints, &server) != 0) {
                perror(gai_strerror(rc));
                exit(-1);
        }

        /* Now we can create the socket and bind it to the local IP and port */
        sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
        if (sockfd == -1) {
                perror("ERROR opening socket");
                exit(-1);
        }
        /* Get rid of "Address already in use" error messages */
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                perror("setsockopt");
                exit(-1);
        }
	rc = bind(sockfd, server->ai_addr, server->ai_addrlen);
        if (rc == -1) {
                perror("ERROR on connect");
                close(sockfd);
                exit(-1);
                // TODO: could use goto here for error cleanup
        }

        free(server);

        /* Time to listen for clients.*/
        listen(sockfd, BACKLOG);
        return sockfd;
}
#endif
