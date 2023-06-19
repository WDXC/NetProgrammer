cmake_minimum_required(VERSION 3.5)
project(Network)

message(STATUS "src this is source dir " ${CMAKE_SOURCE_DIR})

# set different program names
SET(ECHO "echo_server")
SET(CLIENT "test_client")

# set property
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -Wunused")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/build/lib)

# find source cpp files
file(GLOB_RECURSE source_list_echo_server "src/echo_server/*.cpp")
file(GLOB_RECURSE source_list_test_client "src/test_client/*.cpp")

# find include files
include_directories(
    include/echo_server
    include/test_client
    include/Log
)

# generate executable files
# echo_server
add_library(${ECHO} SHARED ${source_list_echo_server})
add_executable(${ECHO}_main ${CMAKE_CURRENT_SOURCE_DIR}/example/echo_server_main.cpp)
target_link_libraries(${ECHO}_main PRIVATE ${ECHO} -levent -lpthread 
    -levent_core)

# test_client
add_library(${CLIENT} ${source_list_test_client})
add_executable(${CLIENT}_main ${source_list}
    ${CMAKE_CURRENT_SOURCE_DIR}/example/test_client_main.cpp)
target_link_libraries(${CLIENT}_main PRIVATE ${CLIENT} -levent -lpthread  -levent_core)


