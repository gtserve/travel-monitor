cmake_minimum_required(VERSION 3.19)
project(syspro-hw2 C)

set(CMAKE_C_STANDARD 99)

add_executable(
        travel_monitor
        src/travelmonitor.c
        src/util.c
        src/msg.c
)

add_executable(
        monitor
        src/monitor.c
        src/msg.c
        )