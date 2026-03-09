#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>

int main(void) {
    struct addrinfo hints;
    struct addrinfo * res; // will point to the results

    memset(&hints, 0, sizeof hints); // fill hints with 0's
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    hints.ai_socktype = SOCK_STREAM; // TCP stream socket

    int status;
    if ((status = getaddrinfo(NULL, "3490", &hints, &res)) != 0) {
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
    int b;
    b = bind(sockfd, res->ai_addr, res->ai_addrlen)
    
    freeaddrinfo(res);

    return 0;
}
