CC=gcc
CFLAGS=-g -Wall -Wextra -I.
DEPS= argparse.h

argparse.o: argparse.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: argparse.o
	$(CC) $(CFLAGS) -o parse main.c argparse.c
