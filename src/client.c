#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>

#include "chat_config.h"

int main(void) {
    printf("My chat room client. Version One.\n");

    // Helper structs for creating socket
    struct addrinfo hints;
    struct addrinfo *res;  // will point to the results of getaddrinfo()

    int status;       // return of getaddrinfo()
    int sockfd = -1;  // socket file descriptor
    int numbytes;
    
    char *server = MYIP;
    
    memset(&hints, 0, sizeof hints);  // fill hints with 0's
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;  // TCP stream socket

    // Get address info for the server
    if ((status = getaddrinfo(server, MYPORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Try each address until we can connect
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;  // Success!
    }

    if (sockfd == -1) {
        fprintf(stderr, "Failed to connect to server\n");
        exit(1);
    }

    freeaddrinfo(res);

    // Input/output loop
    char buf[BUFFERSIZE];
    char response[BUFFERSIZE];

    while (1) {
        // Read user input from stdin
        if (fgets(buf, sizeof buf, stdin) == NULL) {
            break;  // EOF or error
        }

        // Remove newline if present
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }

        // Send command to server
        if (send(sockfd, buf, strlen(buf), 0) == -1) {
            fprintf(stderr, "send error\n");
            break;
        }

        // Receive response from server
        memset(response, 0, sizeof response);
        numbytes = recv(sockfd, response, sizeof response - 1, 0);
        
        if (numbytes == -1) {
            fprintf(stderr, "recv error\n");
            break;
        }

        if (numbytes == 0) {
            fprintf(stderr, "Server disconnected\n");
            break;
        }

        response[numbytes] = '\0';
        
        // Display response
        printf("%s", response);
        
        memset(buf, 0, sizeof buf);
    }

    close(sockfd);
    return 0;
}
