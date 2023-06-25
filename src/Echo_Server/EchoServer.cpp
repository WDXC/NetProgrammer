#include "EchoServer.h"
#include <arpa/inet.h>
#include <err.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

EchoServer::EchoServer() {}

EchoServer::~EchoServer() {}



void EchoServer::on_read_cb(struct bufferevent *bev, void *ctx) {
  struct evbuffer *input = bufferevent_get_input(bev);
  struct evbuffer *output = bufferevent_get_output(bev);

  // 从输入缓冲区读取数据
  size_t len = evbuffer_get_length(input);
  char *data = (char *)malloc(len);
  evbuffer_copyout(input, data, len);

  printf("The message from client is : %s", data);

  // 将数据写入输出缓冲区
  evbuffer_add_buffer(output, input);

  // 释放内存
  free(data);
}

void EchoServer::echo_event_cb(struct bufferevent *bev, short events,
                               void *ctx) {
  if (events & BEV_EVENT_ERROR) {
    perror("Error");
  }
  if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
    bufferevent_free(bev);
  }
}

void EchoServer::on_accpet_cb(struct evconnlistener *listener,
                              evutil_socket_t fd, struct sockaddr *address,
                              int socklen, void *ctx) {
  struct event_base *base = evconnlistener_get_base(listener);
  struct bufferevent *bev =
      bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

  // 设置读回调函数和事件回调函数
  bufferevent_setcb(bev, on_read_cb, NULL, echo_event_cb, NULL);

  // 开始监听读事件
  bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void EchoServer::accept_error_cb(struct evconnlistener *listener, void *ctx) {
  struct event_base *base = evconnlistener_get_base(listener);
  int err = EVUTIL_SOCKET_ERROR();
  fprintf(stderr, "Error %d: %s\n", err, evutil_socket_error_to_string(err));

  event_base_loopexit(base, NULL);
}

void EchoServer::init() {
  struct event_base *base;
  struct evconnlistener *listener;
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
                                     -1, (struct sockaddr *)&sin, sizeof(sin));
  if (!listener) {
    fprintf(stderr, "Could not create listener.\n");
    return;
  }

  evconnlistener_set_error_cb(listener, accept_error_cb);

  event_base_dispatch(base);

  evconnlistener_free(listener);
  event_base_free(base);
}
