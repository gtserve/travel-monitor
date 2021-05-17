# Makefile
# -------------------------------------------------------------------------------------------------
# course:   System Programming
# project:  (#2) Travel Monitor
# name:     George Tservenis
# email:    sdi1500164@di.uoa.gr
# -------------------------------------------------------------------------------------------------


SRC = $(wildcard ./src/*.c)
EXE = travelMonitor
CC  = gcc
CFL = -O2 -g -Wall -Werror

compile:
	$(CC) $(CFL) $(SRC) -o $(EXE)

clean:
	rm -f $(EXE)
