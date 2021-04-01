# Makefile
# ----------------------------------------
# System Programming - Homework 1
# George Tservenis
# sdi1500164@di.uoa.gr
# ----------------------------------------

SRC = main.c
#HDR =
OBJ = main.o
EXE = main.exe
CC  = gcc
CFL = -g -Wall -Werror
#LIB   =

build:
	mkdir ./build
	$(CC) $(CFL) $(SRC) -o ./build/$(EXE)

execute:
	@ ./build/$(EXE)

clean:
	rm -rf ./build
