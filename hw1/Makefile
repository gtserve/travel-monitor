# Makefile
# ----------------------------------------
# System Programming - Homework 1
# George Tservenis
# sdi1500164@di.uoa.gr
# ----------------------------------------

SOURCE = main.c
#HEADER =
OBJECT = main.o
OUT    = main.exe
CC     = gcc
CFLAGS = -g -c -Wall -Werror
#LIBS   =

$(OUT): $(OBJECT)
	$(CC) -g $(OBJECT) -o $@

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -f $(OUT) $(OBJECT)