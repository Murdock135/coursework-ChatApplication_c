/*
Authored by: Qazi Zarif Ul Islam (qzic2d)
Date: Mar 19, 2026

Description:


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>

#include "chat_config.h" 
#include "server/user.h"

// Forward declaration
void handle_command(const char *buf, char *response, char *logged_in_user);

// Handle client commands and send responses
void handle_command(const char *buf, char *response, char *logged_in_user) {
    char command[BUFFERSIZE];
    char arg1[NAMELENGTH];
    char arg2[NAMELENGTH];
    int result;
    
    // Parse the command
    int nargs = sscanf(buf, "%s %s %s", command, arg1, arg2);
    
    // Handle NEWUSER command
    if (strcmp(command, "NEWUSER") == 0) {
        if (nargs < 3) {
            snprintf(response, BUFFERSIZE, "Usage: NEWUSER <username> <password>\n");
            return;
        }
        result = create_user(arg1, arg2);
        if (result == 1) {
            snprintf(response, BUFFERSIZE, "New user account created. Please login.\n");
            printf("New user account created\n");
        } else if (result == 0) {
            snprintf(response, BUFFERSIZE, "Denied. User account already exists.\n");
        } else if (result == -1) {
            snprintf(response, BUFFERSIZE, "Denied. No space for new users.\n");
        } else {
            snprintf(response, BUFFERSIZE, "Denied. Username must be 3-32 chars and password 4-8 chars.\n");
        }
        return;
    }
    
    // Handle LOGIN command
    if (strcmp(command, "LOGIN") == 0) {
        if (nargs < 3) {
            snprintf(response, BUFFERSIZE, "Usage: LOGIN <username> <password>\n");
            return;
        }
        result = login_user(arg1, arg2);
        if (result == 1) {
            strncpy(logged_in_user, arg1, NAMELENGTH - 1);
            snprintf(response, BUFFERSIZE, "login confirmed\n");
            printf("%s login.\n", arg1);
        } else if (result == -1) {
            snprintf(response, BUFFERSIZE, "Denied. User already logged in.\n");
        } else {
            snprintf(response, BUFFERSIZE, "Denied. User name or password incorrect.\n");
        }
        return;
    }
    
    // Handle LOGOUT command
    if (strcmp(command, "LOGOUT") == 0) {
        if (logged_in_user[0] == '\0') {
            snprintf(response, BUFFERSIZE, "Denied. Not logged in.\n");
            return;
        }
        result = logout_user(logged_in_user);
        if (result == 1) {
            snprintf(response, BUFFERSIZE, "%s left.\n", logged_in_user);
            printf("%s logout\n", logged_in_user);
            logged_in_user[0] = '\0';
        } else {
            snprintf(response, BUFFERSIZE, "Denied. Logout failed.\n");
        }
        return;
    }
    
    // Handle SEND command
    if (strcmp(command, "SEND") == 0) {
        if (logged_in_user[0] == '\0') {
            snprintf(response, BUFFERSIZE, "Denied. Please login first.\n");
            return;
        }
        
        // Get the message part (everything after "SEND ")
        char *message_start = strchr(buf, ' ');
        if (message_start == NULL || *(message_start + 1) == '\0') {
            snprintf(response, BUFFERSIZE, "Denied. Empty message.\n");
            return;
        }
        message_start++;  // skip the space
        
        // Format response as: "username: message"
        snprintf(response, BUFFERSIZE, "%s: %s\n", logged_in_user, message_start);
        printf("%s: %s\n", logged_in_user, message_start);
        return;
    }
    
    // Unknown command
    snprintf(response, BUFFERSIZE, "Unknown command. Use NEWUSER, LOGIN, LOGOUT, or SEND.\n");
}


int main(void) {
    init_users();  // Initialize user database

    // Struct holding information of incoming connection
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    // Helper structs for creating socket
    struct addrinfo hints;
    struct addrinfo * res; // will point to the results of getaddrinfo()

    int status; // return of getaddrinfo()
    int sockfd; // return of socket()
    int new_sockfd; // return of accept()
    char * address = NULL; // their address

    memset(&hints, 0, sizeof hints); // fill hints with 0's
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me
    hints.ai_socktype = SOCK_STREAM; // TCP stream socket

    if ((status = getaddrinfo(address, MYPORT, &hints, &res)) != 0) {
        fprintf(stderr, "gai_error: %s\n", gai_strerror(status));
        exit(1);
    }

    // create socket and try each addrinfo result
    // ----------------------
    // Loop through results and use the first one that works
    sockfd = -1;
    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            continue;
        }

        // bind socket
        // -----------------
        // This is the server. So bind socket to port and then listen in
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;  // Success!
    }

    if (sockfd == -1) {
        fprintf(stderr, "socket/bind error\n");
        exit(1);
    }

    printf("socket descriptor: %d\n", sockfd);

    // listen
    // --------
    if (listen(sockfd, BACKLOG) == -1) {
        fprintf(stderr, "listen error\n");
        exit(1);
    }

    printf("My chat room server. Version One.\n");

    // accept and handle connections
    // --------------
    while (1) {
        addr_size = sizeof their_addr;
        new_sockfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
        if (new_sockfd == -1) {
            fprintf(stderr, "accept error\n");
            continue;
        }

        // Track which user is logged in on this connection
        char logged_in_user[NAMELENGTH];
        memset(logged_in_user, 0, NAMELENGTH);

        // receive data from client
        // -------------------------
        char buf[BUFFERSIZE];
        char response[BUFFERSIZE];
        int numbytes;
        
        while ((numbytes = recv(new_sockfd, buf, sizeof buf - 1, 0)) > 0) {
            buf[numbytes] = '\0'; // null-terminate the received data
            
            // Parse command and generate response
            memset(response, 0, sizeof response);
            handle_command(buf, response, logged_in_user);
            
            // Send response back to client
            send(new_sockfd, response, strlen(response), 0);
            
            memset(buf, 0, sizeof buf);
        }

        if (numbytes == 0) {
            printf("Client disconnected\n");
        } else if (numbytes == -1) {
            fprintf(stderr, "recv error\n");
        }

        close(new_sockfd);
    }
    
    freeaddrinfo(res);
    close(sockfd);

    return 0;
}
