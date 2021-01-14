
CC=gcc
CFLAGS=-g -Wall -std=c99
CCLINK=$(CC)
OBJS=grades.o libgrades.so
EXEC=libgrades.so


libgrades.so: grades.o
	$(CCLINK) -shared grades.o -o libgrades.so -l linked-list -L.

grades.o: grades.c grades.h linked-list.h 
	$(CC) $(CFLAGS) -c -fpic grades.c

clean:
	rm -rf grades.o  libgrades.so
