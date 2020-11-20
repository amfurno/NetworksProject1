cmake_minimum_required(VERSION 3.10.2)
include_directories(.)

project(server2)
project(client2)


set(CMAKE_CXX_STANDARD 11)



add_executable(server2 ServerSourceCode/server.cpp ClientSourceCode/gremlin.cpp unp.cpp)
add_executable(client2 ClientSourceCode/client.cpp ClientSourceCode/gremlin.cpp unp.cpp)