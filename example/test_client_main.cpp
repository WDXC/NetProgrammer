#include "test_client.h"
#include "base_log.h"

using namespace test_client;

int main(int argc, char** argv) {
    Client* amp = new Client();
    InitInvocationName(argv[0]);

    amp->init();
}
