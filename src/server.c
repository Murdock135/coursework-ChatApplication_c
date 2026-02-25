#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>

int main(void) {
    struct addrinfo hints;
    struct addrinfo * head; // will point to the head of addrinfo-linked-list
    int status;
    
    memset(&hints, 0, sizeof hints); // Ensure everything is empty (0)
    status = getaddrinfo(NULL, "3490", &hints, &head); // returns a status
    if (status != 0 ) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(status));
        exit(1);
    }
    else printf("Success!");
    return 0;
}
