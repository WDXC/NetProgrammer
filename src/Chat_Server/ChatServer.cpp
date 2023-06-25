#include "ChatServer.h"
#include <string.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>

namespace Chat {

ChatServer::ChatServer() {}

ChatServer::~ChatServer() {}

void ChatServer::on_read_cb(struct bufferevent* bev, void* ctx) {
    struct evbuffer* input = bufferevent_get_input(bev);
    struct evbuffer* output = bufferevent_get_output(bev);

    // read from the buffer
    size_t len = evbuffer_get_length(input);
    char* data = new char[len+1];
    evbuffer_copyout(input, data, len);

    // write data to buffer
    evbuffer_add_buffer(output, input);

    // release memory
    delete [] data;
}

void ChatServer::echo_event_cb(struct bufferevent* bev, short events, void* ctx) {
    if (events & BEV_EVENT_ERROR) {
        perror("Error");
    }
    if( events & (BEV_EVENT_EOF | BEV_EVENT_ERROR) ) {
        bufferevent_free(bev);
    }
}

void ChatServer::on_accpet_cb(struct evconnlistener* listener,
        evutil_socket_t fd, struct sockaddr* address,
        int socklen, void* ctx) {
    struct event_base* base = evconnlistener_get_base(listener);
    struct bufferevent* bev = 
        bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    // set read callback and event callback
    bufferevent_setcb(bev, on_read_cb, NULL, echo_event_cb, NULL);

    // start monitor the event
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void ChatServer::accept_error_cb(struct evconnlistener* listener, void* ctx) {
    struct event_base* base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Error %d:%s\n", err, evutil_socket_error_to_string(err));
    event_base_loopexit(base, NULL);
}

void ChatServer::init() {
    struct event_base* base;
    struct evconnlistener* listener;
    struct sockaddr_in sin;

    int port = 8888;
    
    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize event base.\n");
        return;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(base, on_accpet_cb, NULL,
            LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
            -1, (struct sockaddr*)&sin, sizeof(sin));

    if (!listener) {
        fprintf(stderr, "Could not create listener.\n");
        return;
    }

    evconnlistener_set_error_cb(listener, accept_error_cb);

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);
}


} // namespace Chat
