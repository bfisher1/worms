#makes the queueTest file
CC = gcc
CFLAGS = -Wall -std=c99 -g

queueTest : queueTest.o queue.o
	$(CC) $(CFLAGS) -o $@ $^ 

queue.o : queue.h queue.c

queueTest.o : queueTest.c queue.h


clean:
	rm -f main.o queue.o queueTest
