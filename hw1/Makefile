# Makefile
# -----------------------------------------------------------------------------
# System Programming - Homework 1
# George Tservenis
# sdi1500164@di.uoa.gr
# -----------------------------------------------------------------------------


#SRC = 	./src/main.c ./src/bloomfilter.c ./src/commands.c ./src/data.c ./src/handler.c ./src/hash.c \
# 		./src/hashtable.c ./src/skiplist.c ./src/util.c
SRC = $(wildcard ./src/*.c)
#OBJ = main.o
EXE = vaccineMonitor
CC  = gcc
CFL = -O2 -g -Wall -Werror

compile:
	$(CC) $(CFL) $(SRC) -lm -o $(EXE)

clean:
	rm -f $(EXE)
