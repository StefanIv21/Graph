CC = gcc -std=c99
CFLAGS= -g -Wall -lm

build:task2

task2:graph.c
	$(CC) -std=c99 -g -o tema3  graph.c $(CFLAGS)

.PHONY:clean
clean:
	rm -f tema3
	rm -f *.out
