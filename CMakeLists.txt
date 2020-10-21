cmake_minimum_required(VERSION 3.10.2)
project(server)
project(client)

set(CMAKE_CXX_STANDARD 11)

add_executable(server Server/server.cpp)
add_executable(client Client/client.cpp Client/gremlin.cpp)