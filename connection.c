#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/epoll.h>

#include "epollinterface.c"
#include "server_socket.c"

struct data_buffer_entry {
    int is_close_message;
    char* data;
    int current_offset;
    int len;
    struct data_buffer_entry* next;
};

struct connection_closure {
    void (*on_read) (void* closure,char* buffer,int len);
    void* on_read_closure;
    void (*on_close) (void* closure);
    void* on_close_closure;

    struct data_buffer_entry* write_buffer;
};

connection_really_close(struct epoll_event_handler* self) {
    struct connection_closure* closure = (struct connection_closure*) self->closure;
    struct data_buffer_entry* next;
    while(closure->write_buffer != NULL) {
        next = closure->write_buffer->next;
        if(!closure->write_buffer->is_close_message) {
            epoll_add_to_free_list(closure->write_buffer->data);
        }
        epoll_add_to_free_list(closure->write_buffer);
        closure->write_buffer = next;
    }

    epoll_remove_handler(self);
    close(self->fd);
    epoll_add_to_free_list(self->closure);
    epoll_add_to_free_list(self);
    printf("freed connection %p",self);
}

void connection_on_out_event(struct epoll_event_handler* self) {
    struct connection_closure* closure = (struct connection_closure*) self->closure;
    int written;
    int to_write;
    struct data_buffer_entry* temp;

    while(closure->write_buffer != NULL) {
        if(closure->write_buffer->is_close_message){
            connection_really_close(self);
            return;
        }
    }

    
}

void connection_handle_event(struct epoll_event_handler* self,uint32_t events) {
    if(events & EPOLLOUT) {
        connection_on_out_event(self);
    }

    if(events & EPOLLIN) {
        connection_on_in_event(self);
    }

    if((events & EPOLLERR) | (events & EPOLLHUP) | (events & EPOLLRDHUP)) {
        connection_on_close_event(self);
    }
}

struct epoll_event_handler* create_connection(int client_socket_fd) {
    make_socket_non_blocking(client_socket_fd);

    struct connection_closure* closure = malloc(sizeof(struct connection_closure));
    closure->write_buffer = NULL;

    struct epoll_event_handler* result = malloc(sizeof(struct epoll_event_handler));
    printf("Created connection epoll handler %p",result);
    result->fd = client_socket_fd;
    result->handle = connection_handle_event;
    result->closure = closure;

    epoll_add_handler(result,EPOLLIN | EPOLLRDHUP | EPOLLET | EPOLLOUT);

    return result;
}