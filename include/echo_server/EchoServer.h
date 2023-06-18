#ifndef _ECHO_SERVER_H_
#define _ECHO_SERVER_H_

#include <event.h>

class EchoServer {
public:
  EchoServer();
  ~EchoServer();

  void init();
  struct client;

  static int setnonblock(int fd);
  static void on_read(int fd, short ev, void *arg);
  static void on_accpet(int fd, short ev, void *arg);

  static int SERVER_PORT;
};

#endif
