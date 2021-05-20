# Makefile
# -------------------------------------------------------------------------------------------------
# course:   System Programming
# project:  (#2) Travel Monitor
# name:     George Tservenis
# email:    sdi1500164@di.uoa.gr
# -------------------------------------------------------------------------------------------------

SRC = src/util.c src/msg.c
#SRC = $(wildcard ./src/*.c)
EXE = travelMonitor
CC  = gcc
CFL = -O2 -g -Wall -Werror

all: compile_monitor compile_travelmonitor

compile_monitor:
	$(CC) $(CFL) ./src/monitor.c $(SRC) -o ./monitor

compile_travelmonitor:
	$(CC) $(CFL) ./src/travelmonitor.c $(SRC) -o $(EXE)

clean:
	rm -f ./travelMonitor ./monitor
