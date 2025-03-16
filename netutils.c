#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

#include "netutils.h"

void make_socket_non_blocking(int socekt_fd) {
    int flags;
    flags = fcntl(socekt_fd,F_GETFL,0);

    if(flags == -1) {
        fprintf(stderr,"Couldn't get socket flags\n");
        exit(1);
    }

    flags |= O_NONBLOCK;
    if(fcntl(socekt_fd,F_SETFL,flags) == -1) {
        fprintf(stderr,"Couldn't set socket flags\n");
        exit(1);
    } 
}

int connect_to_backend(char* backend_host,char* backend_port_str) {
    struct addrinfo hints;
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int getaddrinfo_error;
    struct addrinfo* addrs;
    getaddrinfo_error = getaddrinfo(backend_host,backend_port_str,&hints,&addrs);
    if(getaddrinfo_error != 0){
        if(getaddrinfo_error == EAI_SYSTEM ){
            printf("Couldn't find backend\n");
        } else {
            printf("Couldn't find backend: %s\n",gai_strerror(getaddrinfo_error));
        }
        exit(1);
    }

    int backend_socket_fd;
    struct addrinfo* addrs_iter;
    for(addrs_iter = addrs;addrs_iter != NULL;addrs_iter = addrs_iter->ai_next) {
        backend_socket_fd = socket(addrs_iter->ai_family,addrs_iter->ai_socktype,addrs_iter->ai_protocol);
        if(backend_socket_fd == -1) {
            continue;
        }

        if(connect(backend_socket_fd,addrs_iter->ai_addr,addrs_iter->ai_addrlen) != -1) {
            break;
        }

        close(backend_socket_fd);
    }

    if(addrs_iter == NULL) {
        printf("Couldn't connect to backend\n");
        exit(1);
    }

    freeaddrinfo(addrs);

    return backend_socket_fd;
}