#include "EchoServer.h"
#include "base_log.h"
#include <arpa/inet.h>
#include <err.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int EchoServer::SERVER_PORT = 5555;

struct EchoServer::client {
  struct event ev_read;
};

EchoServer::EchoServer() {}

EchoServer::~EchoServer() {}

int EchoServer::setnonblock(int fd) {
  int flags;

  flags = fcntl(fd, F_GETFL);
  if (flags < 0) {
    return flags;
  }
  flags |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) < 0) {
    return -1;
  }
  return 0;
}

void EchoServer::on_read(int fd, short ev, void *arg) {
  struct client *client = (struct client *)arg;
  u_char buf[8196];
  int len, wlen;

  len = read(fd, buf, sizeof(buf));
  if (len == 0) {
    LOG(INFO) << "client disconnected";
    close(fd);
    event_del(&client->ev_read);
    free(client);
    return;
  } else if (len < 0) {
    LOG(INFO) << "Socket failure, disconnected client: %s" << strerror(errno);
    close(fd);
    event_del(&client->ev_read);
    free(client);
    return;
  }

  wlen = write(fd, buf, len);
  if (wlen < len) {
    LOG(INFO) << "Short write, not all data echoed back to client.";
  }
}

void EchoServer::on_accpet(int fd, short ev, void *arg) {
  int client_fd;
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  struct client *client;

  /* Accept the new connection */
  client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
  if (client_fd == -1) {
    warn("accept failed");
    return;
  }

  if (EchoServer::setnonblock(client_fd) < 0) {
    warn("failed to set client socket non-blocking");
  }

  client = (struct client *)calloc(1, sizeof(*client));

  if (client == NULL) {
    err(1, "malloc failed");
  }

  event_set(&client->ev_read, client_fd, EV_READ | EV_PERSIST, on_read, client);

  event_add(&client->ev_read, NULL);

  LOG(INFO) << "Accept connection from %s " << inet_ntoa(client_addr.sin_addr);
}

void EchoServer::init() {
  int listen_fd;
  struct sockaddr_in listen_addr;
  int reuseaddr_on = 1;

  struct event ev_accept;
  event_init();

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0)
    err(1, "listen failed");
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on,
                 sizeof(reuseaddr_on)) == -1)
    err(1, "setsockopt failed");
  memset(&listen_addr, 0, sizeof(listen_addr));
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_addr.s_addr = INADDR_ANY;
  listen_addr.sin_port = htons(SERVER_PORT);
  if (bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
    err(1, "bind failed");
  if (listen(listen_fd, 5) < 0)
    err(1, "listen failed");

  /* Set the socket to non-blocking, this is essential in event
   * based programming with libevent. */
  if (setnonblock(listen_fd) < 0)
    err(1, "failed to set server socket to non-blocking");

  /* We now have a listening socket, we create a read event to
   * be notified when a client connects. */
  event_set(&ev_accept, listen_fd, EV_READ | EV_PERSIST, EchoServer::on_accpet, NULL);
  event_add(&ev_accept, NULL);

  /* Start the libevent event loop. */
  event_dispatch();
}
