# Makefile
# -----------------------------------------------------------------------------
# System Programming - Homework 1
# George Tservenis
# sdi1500164@di.uoa.gr
# -----------------------------------------------------------------------------

SRC = src/main.c
#HDR =
OBJ = main.o
EXE = vaccineMonitor
CC  = gcc
CFL = -g -Wall -Werror
#LIB   = -lm -lcrypto

build:
	mkdir ./build
	$(CC) $(CFL) $(SRC) -o ./build/$(EXE)

execute:
	@ ./build/$(EXE)

clean:
	rm -rf ./build
