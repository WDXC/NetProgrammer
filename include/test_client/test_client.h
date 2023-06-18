#ifndef _TEST_CLIENT_H_
#define _TEST_CLIENT_H_

#include "base_log.h"

namespace test_client {

#define  CLIENT_PORT 5555
#define  MAX_IP_SIZE 15
#define MAX_BUFFER 4096


class Client {
public:
    Client();
    ~Client();

    void init();
};

} // namespace test_client

#endif
