#include "test_client.h"
#include <arpa/inet.h> // sockaddr_in
#include <string>
#include <sys/socket.h> // socket, bind, connect
#include <sys/types.h>
#include <unistd.h>

using std::cout;
using std::cin;
using std::string;

namespace test_client {
Client::Client() {}

Client::~Client() {}

void Client::init() {
  // create a file desciptor
  // create a socket
  // config address
  // bind a port
  // connect
  // close
  int client_fd = 0;
  int ret = 0;
  client_fd = socket(PF_INET, SOCK_STREAM, 0);
  char IpInfo[MAX_IP_SIZE + 1];
  char message[MAX_BUFFER+1];
  if (ret < 0) {
    LOG(ERROR) << "failed to creat a socket, errno is : %d" << errno;
    exit(1);
  }
  sockaddr_in *client_addr = NULL;
  memcpy(IpInfo, "121.43.147.75", MAX_IP_SIZE);
  client_addr->sin_port = htons(CLIENT_PORT);
  client_addr->sin_family = AF_INET;
  client_addr->sin_addr.s_addr = inet_addr(IpInfo);

  socklen_t addrlen = sizeof(*client_addr);
  ret = connect(client_fd, (const struct sockaddr *)(&client_addr),
             addrlen);
  if (ret < 0) {
      LOG(ERROR) << "failed to connect , errno is : %d" << errno;
      exit(1);
  }

  while (1) {
      cout << "Input your message to server: ";
      string test = nullptr;
      cin >> test;
      memcpy(message, test.c_str(), test.size());
      write(client_fd, message, sizeof(message));
      ret = read(client_fd, message, sizeof(message)-1);
      bzero(message, sizeof(message));
      cout << "Message from server is : %s" << message;
  }
}
} // namespace test_client
