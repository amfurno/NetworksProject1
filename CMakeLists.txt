cmake_minimum_required(VERSION 3.10.2)
include_directories(.)

project(server)
project(client)
project(unp)

set(CMAKE_CXX_STANDARD 11)



add_executable(server Server/server.cpp unp.cpp)
add_executable(client Client/client.cpp Client/gremlin.cpp unp.cpp)