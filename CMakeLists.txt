cmake_minimum_required(VERSION 3.5)
project(basic)

message(STATUS "src this is source dir " ${CMAKE_SOURCE_DIR})


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

# 设置可执行文件名和源文件列表
file(GLOB_RECURSE source_list "src/*.cpp")

# 添加头文件目录
include_directories(include)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${source_list})
target_link_libraries(${PROJECT_NAME} -levent -lpthread)

