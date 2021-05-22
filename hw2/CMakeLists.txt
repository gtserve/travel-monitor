cmake_minimum_required(VERSION 3.19)
project(syspro-hw2 C)

set(CMAKE_C_STANDARD 99)

add_executable(
        travel_monitor
        src/travelmonitor.c
        src/msg.c
        src/util.c
        src/commands.c
        src/data.c
        src/hashtable.c
        src/hash.c
        src/bloomfilter.c
        src/skiplist.c
        src/comm_protocol.c
)

add_executable(
        monitor
        src/monitor.c
        src/msg.c
        src/util.c
        src/commands.c
        src/data.c
        src/hashtable.c
        src/hash.c
        src/bloomfilter.c
        src/skiplist.c
        src/comm_protocol.c
)

add_executable(test src/test/test.c src/util.c)

target_link_libraries(travel_monitor -lm)

target_link_libraries(monitor -lm)
