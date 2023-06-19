#ifndef _TEST_CLIENT_H_
#define _TEST_CLIENT_H_

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>

namespace test_client {

#define CLIENT_PORT 5555
#define MAX_IP_SIZE 15
#define MAX_BUFFER 4096

class Client {
public:
  Client();
  ~Client();

  static void read_cb(struct bufferevent *bev, void *ctx);
  static void event_cb(struct bufferevent *bev, short events, void *ctx);
  static void stdin_read_cb(evutil_socket_t fd, short event, void *arg);

  void init();
};

} // namespace test_client

#endif
