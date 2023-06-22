#include "test_client.h"
#include <arpa/inet.h> // sockaddr_in
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h> // socket, bind, connect
#include <sys/types.h>
#include <unistd.h>

using std::string;

namespace test_client {
Client::Client() {}

Client::~Client() {}

void Client::stdin_read_cb(evutil_socket_t fd, short event, void *arg) {
  char message[1024];
  struct bufferevent *bev = static_cast<struct bufferevent *>(arg);
  struct evbuffer *output = bufferevent_get_output(bev);

  if (bufferevent_getfd(bev) == -1) {
    // 如果尚未连接到服务器，则不发送数据
    return;
  }

  if (fgets(message, sizeof(message), stdin) != nullptr) {
    evbuffer_add(output, message, strlen(message));
    bufferevent_flush(bev, EV_WRITE, BEV_FLUSH);
  } else {
    event_base_loopbreak(event_get_base(static_cast<struct event *>(arg)));
  }
}

void Client::read_cb(struct bufferevent *bev, void *ctx) {
  struct evbuffer *input = bufferevent_get_input(bev);
  size_t len = evbuffer_get_length(input);
  char *data = new char[len + 1];
  evbuffer_copyout(input, data, len);
  data[len] = '\0';

  evbuffer_drain(input, len);
  
  std::cout << "Received: " << data << std::endl;
  delete[] data;
}

void Client::event_cb(struct bufferevent *bev, short events, void *ctx) {
  if (events & BEV_EVENT_CONNECTED) {
    std::cout << "Connected to server." << std::endl;
  } else if (events & (BEV_EVENT_ERROR | BEV_EVENT_EOF)) {
    if (events & BEV_EVENT_ERROR) {
      std::cerr << "Error" << std::endl;
    }
    std::cout << "Connection closed." << std::endl;
    bufferevent_free(bev);
  }
}

void Client::init() {
  struct event_base *base = event_base_new();
  if (!base) {
    std::cerr << "Could not initialize event base." << std::endl;
    return;
  }

  struct bufferevent *bev = bufferevent_socket_new(
      base, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
  bufferevent_setcb(bev, read_cb, nullptr, event_cb,
                    base); // 设置读取回调函数和事件回调函数

  const char *server_ip = "127.0.0.1";
  int server_port = 8888;
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(server_ip);
  sin.sin_port = htons(server_port);

  if (bufferevent_socket_connect(bev, reinterpret_cast<struct sockaddr *>(&sin),
                                 sizeof(sin)) < 0) {
    perror("Error connecting to server");
    bufferevent_free(bev);
    return;
  }

  struct event *stdin_event =
      event_new(base, fileno(stdin), EV_READ | EV_PERSIST, stdin_read_cb, bev);
  event_add(stdin_event, nullptr);

  bufferevent_enable(bev, EV_READ);

  event_base_dispatch(base);

  bufferevent_free(bev);
  event_base_free(base);
  event_free(stdin_event);
}
} // namespace test_client
