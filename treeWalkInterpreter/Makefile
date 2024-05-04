CC = gcc
CFLAGS = -g

ALL_CFLAGS = $(CFLAGS) -Wall -Werror

.PHONY: all
all: main

main: main.o
	$(CC) -o main main.o

main.o: main.c
	$(CC) -c main.c $(ALL_CFLAGS) 

.PHONY: clean
clean:
	$(RM) -f *.o
	$(RM) main

