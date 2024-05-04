CC = gcc
CFLAGS = -g

ALL_CFLAGS = $(CFLAGS) -Wall -Werror

.PHONY: all
all: jlox

jlox: main.o
	$(CC) -o jlox main.o

main.o: main.c
	$(CC) -c main.c $(ALL_CFLAGS) 

.PHONY: run
run: main
	./main touch.lox

.PHONY: clean
clean:
	$(RM) -f *.o
	$(RM) jlox

