#include "test_client.h"
#include <iostream>

using namespace test_client;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " ip port" << std::endl;
        return 0;
    }
    Client* amp = new Client();

    amp->init(argv[1], argv[2]);
}
