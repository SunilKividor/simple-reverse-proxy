#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "epollinterface.c"
#include "server_socket.c"

#define MAX_LISTEN_BACKLOG 1
#define BUFFER_SIZE 4096

int main(int argc,char *argv[]) {

    if(argc != 4) {
        fprintf(stderr,"Usage: %s <server_port> <backend_addr> <backend_port>\n", argv[0]);
        exit(1);
    }

    char* server_port_str = argv[1];
    char* backend_addr = argv[2];
    char* backend_port_str = argv[3];

    epoll_init();

    struct epoll_event_handler* server_socket_event_handler;
    server_socket_event_handler = create_server_socket_handler(server_port_str,backend_addr,backend_port_str);



    return 0;
}

