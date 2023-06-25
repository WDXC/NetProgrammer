#ifndef _CHAT_SERVER_H_
#define _CHAT_SERVER_H_

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

namespace Chat {

class ChatServer {
public:
  ChatServer();
  ~ChatServer();

  void init();

  static void conn_writecb(struct bufferevent *, void *);
  static void on_read_cb(struct bufferevent *bev, void *ctx);
  static void echo_event_cb(struct bufferevent *bev, short events, void *ctx);
  static void accept_error_cb(struct evconnlistener *listener, void *ctx);
  static void on_accpet_cb(struct evconnlistener *listener, evutil_socket_t fd,
                            struct sockaddr *address, int socklen, void *ctx);
};

} // namespace Chat

#endif
