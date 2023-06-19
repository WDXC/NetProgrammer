#ifndef _ECHO_SERVER_H_
#define _ECHO_SERVER_H_

#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/listener.h>

class EchoServer {
public:
  EchoServer();
  ~EchoServer();

  void init();
  struct client;

  static int setnonblock(int fd);
  static void on_read_cb(struct bufferevent *bev, void *ctx);
  static void echo_event_cb(struct bufferevent *bev, short events, void *ctx);
  static void accept_error_cb(struct evconnlistener *listener, void *ctx);
  static void on_accpet_cb(struct evconnlistener *listener, evutil_socket_t fd,
                            struct sockaddr *address, int socklen, void *ctx);

  static int SERVER_PORT;
};

#endif
