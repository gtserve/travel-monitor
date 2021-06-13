cmake_minimum_required(VERSION 3.19)
project(syspro-hw3 C)

set(CMAKE_C_STANDARD 99)

add_executable(
        travelMonitorClient
        src/tm_client.c
        src/msg.c
        src/util.c
        src/commands.c
        src/data.c
        src/hashtable.c
        src/hash.c
        src/bloomfilter.c
        src/skiplist.c
        src/comm_protocol.c
        src/handler.c
        src/sockets.c
)

add_executable(
        monitorServer
        src/tm_server.c
        src/msg.c
        src/util.c
        src/commands.c
        src/data.c
        src/hashtable.c
        src/hash.c
        src/bloomfilter.c
        src/skiplist.c
        src/comm_protocol.c
        src/sockets.c
        src/thpool.c
        src/task_cqueue.c
)

target_link_libraries(travelMonitorClient -lm)

target_link_libraries(monitorServer -lm -lpthread)
