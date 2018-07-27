CC = gcc
CFLAGS = -Wall -std=c99 -g 

ArrayListTest : ArrayList.o ArrayListTest.o
	$(CC) $(CFLAGS) -o $@ $^ -lm -fprofile-arcs -ftest-coverage 

ArrayList.o : ArrayList.c ArrayList.h

ArrayListTest.o : ArrayListTest.c ArrayList.h

clean:
	rm -f ArrayList.o ArrayListTest.o

