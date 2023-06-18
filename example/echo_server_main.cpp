#include "EchoServer.h"
#include "base_log.h"

int main(int argc, char** argv) {
    EchoServer* test_echo = new EchoServer();
    InitInvocationName(argv[0]);

    test_echo->init();
}
