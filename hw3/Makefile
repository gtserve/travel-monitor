# Makefile
# -------------------------------------------------------------------------------------------------
# course:   System Programming
# project:  (#3) Travel Monitor (sockets, threads)
# name:     George Tservenis
# email:    sdi1500164@di.uoa.gr
# -------------------------------------------------------------------------------------------------


CC  = gcc
CFL = -O2 -g

LS1 = ./src/tm_client.c ./src/tm_server.c ./src/task_queue.c ./src/handler.c
LS2 = ./src/tm_client.c ./src/tm_server.c ./src/task_queue.c ./src/thpool.c

SRC1:=$(filter-out $(LS1), $(wildcard ./src/*.c))
SRC2:=$(filter-out $(LS2), $(wildcard ./src/*.c))

LIB1 = -lm -lpthread
LIB2 = -lm

all: compile_server compile_client

compile_server:
	$(CC) $(CFL) ./src/tm_server.c $(SRC1) -o ./monitorServer $(LIB1)

compile_client:
	$(CC) $(CFL) ./src/tm_client.c $(SRC2) -o ./travelMonitorClient $(LIB2)

clean:
	rm -f ./travelMonitorClient ./monitorServer
