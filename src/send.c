#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>

#include "chat_config.h"

int main(void) {
    struct addrinfo hints;
    struct addrinfo * res; // will point to the results
    char * port = "3490";
    char * address = NULL;

    memset(&hints, 0, sizeof hints); // fill hints with 0's
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    hints.ai_socktype = SOCK_STREAM; // TCP stream socket

    int status;
    if ((status = getaddrinfo(address, port, &hints, &res)) != 0) {
        fprintf(stderr, "gai_error: %s\n", gai_strerror(status));
        exit(1);
    }

    // create socket
    // ------------
    // socket(int domain, int type, int protocol)
    // int socket returns a socket descriptor
    int sockfd;
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    printf("socket descriptor: %d\n", sockfd);

    // bind socket
    // -------------
    // This is the server. So bind socket to port and then listen in
    // int bind(int sockfd, struct sockaddr *myaddr, int addrlen)
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    // connect socket to (address, port) kept in res->ai_addr
    int connection_status = connect(sockfd, res->ai_addr, res->ai_addrlen);
    printf("Connection status: %d\n", connection_status);
    
    freeaddrinfo(res);

    return 0;
}
turn 0;
}
