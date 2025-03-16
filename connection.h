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

extern struct epoll_event_handler* create_connection(int socket_fd);

extern void connection_write(struct epoll_event_handler* self,char* data,int len);

extern void connection_close(struct epoll_event_handler* self);