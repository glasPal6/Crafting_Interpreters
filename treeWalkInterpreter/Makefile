CC = gcc
CFLAGS = -g

ALL_CFLAGS = $(CFLAGS) -Wall -Werror

.PHONY: all
all: jlox

jlox: jlox.o
	$(CC) -o jlox jlox.o

jlox.o: jlox.c
	$(CC) -c jlox.c $(ALL_CFLAGS) 

.PHONY: run
run: jlox
	./jlox touch.lox

.PHONY: clean
clean:
	$(RM) -f *.o
	$(RM) jlox

