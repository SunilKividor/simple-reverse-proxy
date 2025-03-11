#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/epoll.h>

int epoll_fd;

struct epoll_event_handler {
    int fd;
    void (*handle)(struct epoll_event_handler*, uint32_t);
    void* closure;
};

void epoll_init() {
    epoll_fd =  epoll_create1(0);
    if(epoll_fd == -1) {
        perror("Couldn't create epoll FD");
        exit(1);
    }
}

void epoll_add_handler(struct epoll_event_handler* handler,uint32_t event_mask) {
    struct epoll_event event;

    memset(&event,0,sizeof(struct epoll_event));

    event.data.ptr = handler;
    event.events = event_mask;

    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,handler->fd,&event) == -1) {
        fprintf(stderr,"Could not register server socket with epoll");
        exit(1);
    }
}