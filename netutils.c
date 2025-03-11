#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>

void make_socket_non_blocking(int socekt_fd) {
    int flags;
    flags = fcntl(socekt_fd,F_GETFL,0);

    if(flags == -1) {
        fprintf(stderr,"Couldn't get socket flags");
        exit(1);
    }

    flags |= O_NONBLOCK;
    if(fcntl(socekt_fd,F_SETFL,flags) == -1) {
        fprintf(stderr,"Couldn't set socket flags");
        exit(1);
    } 
}