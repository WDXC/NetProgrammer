cmake_minimum_required(VERSION 3.5)
project(Network)

message(STATUS "src this is source dir " ${CMAKE_SOURCE_DIR})

# set different program names
SET(ECHO "echo_server")
SET(CLIENT "test_client")
SET(CHAT "chat_server")

# set property
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -Wunused")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/build/lib)

# find source cpp files
file(GLOB_RECURSE source_list_echo_server "src/Echo_Server/EchoServer.cpp")
file(GLOB_RECURSE source_list_test_client "src/Echo_Server/test_client.cpp")
file(GLOB_RECURSE source_list_chat_server "src/Chat_Server/ChatServer.cpp")

# find include files
include_directories(
    include/Echo_Server
    include/Chat_Server
    include/Log
)

# generate executable files
# echo_server
add_library(${ECHO} SHARED ${source_list_echo_server})
add_executable(${ECHO}_main ${CMAKE_CURRENT_SOURCE_DIR}/example/Echo_Server/echo_server_main.cpp)
target_link_libraries(${ECHO}_main PRIVATE ${ECHO} -levent -lpthread 
    -levent_core)

# test_client
add_library(${CLIENT} SHARED ${source_list_test_client})
add_executable(${CLIENT}_main ${CMAKE_CURRENT_SOURCE_DIR}/example/Echo_Server/test_client_main.cpp)
target_link_libraries(${CLIENT}_main PRIVATE ${CLIENT} -levent -lpthread  -levent_core)

add_library(${CHAT} SHARED ${source_list_chat_server})
add_executable(${CHAT}_main
    ${CMAKE_CURRENT_SOURCE_DIR}/example/Chat_Server/ChatServer_main.cpp)
target_link_libraries(${CHAT}_main PRIVATE ${CHAT} -levent -lpthread  -levent_core)
