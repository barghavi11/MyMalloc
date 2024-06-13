CC = gcc
CFLAGS = -Wall -fsanitize=address,undefined -std=c99 -O2

all: memtest memgrind actualmalloc mytests clean

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

memtest.o: memtest.c mymalloc.h
	$(CC) $(CFLAGS) -c memtest.c

memtest: memtest.o mymalloc.o
	$(CC) $(CFLAGS) memtest.o mymalloc.o -o memtest

actualmalloc: memgrind.o mymalloc.o
	$(CC) $(CFLAGS) -DREALMALLOC memgrind.o mymalloc.o -o actualmalloc

mytests.o: mytests.c mymalloc.h
	$(CC) $(CFLAGS) -c mytests.c

mytests: mytests.o mymalloc.o 
	$(CC) $(CFLAGS) mytests.o mymalloc.o -o mytests

memgrind: memgrind.o mymalloc.o 
	$(CC) $(CFLAGS) memgrind.o mymalloc.o -o memgrind

clean:
	rm *.o  

