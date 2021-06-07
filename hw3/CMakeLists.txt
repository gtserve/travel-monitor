cmake_minimum_required(VERSION 3.19)
project(syspro-hw3 C)

set(CMAKE_C_STANDARD 99)

add_executable(
        tm_client
        src/tm_client.c
)

add_executable(
        tm_server
        src/tm_server.c
)
