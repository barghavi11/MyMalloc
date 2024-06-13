#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef REALMALLOC
#include "mymalloc.h"
#endif
void t1();
void t2();
void t3();
void t4();
void t5();
void t6();
void t7();


#define MEMSIZE 4096

int main(){
	t1();
	t2();
	t3();
	t4();
	t5();
	t6();
	t7();
}

void t1(){

	// malloc a pointer that is the maximum size of the heap
	void *ptr = malloc(MEMSIZE-8);
	printf("\nTest 1: \nMalloc a pointer that is the maximum heap size\n");
	if (ptr != NULL){
		printf("Allocated %d bytes for ptr in heap\n\n", MEMSIZE-8);
	}

	printf("Now try to malloc() a pointer of 1 byte when the heap is full\n");
	// try to malloc another singular byte - should fail as there is no space in heap
	void *x = malloc(1);
	if (x != NULL){
		printf("Allocated %d bytes for ptr in heap\n", MEMSIZE-8);
	}

	// free the pointer
	free(ptr);

	printf("Is memory leaky: %d\n\n", isMemoryLeaky());
	// 0 = memory is not leaky, 1 = memory is leaky
}

void t2(){
	// malloc 0 bytes

	printf("Test 2: \nTry to malloc() 0 bytes \n");
	void *ptr = malloc(0);

	// check if it has been allocated (should not be allocated)
	if (ptr != NULL){
		printf("Successful allocation of 0 bytes");
	}

	printf("Is memory leaky: %d\n\n", isMemoryLeaky());
}


void t3(){

	void *ptr = malloc(10);
	printf("Test 3: \nTry to free a pointer in the middle of the block\n");

	// try to free a pointer in the middle of the block
	free(ptr + 1);


	free(ptr);

	printf("Is memory leaky: %d\n\n", isMemoryLeaky());
}

void t4(){
	// double free 
	printf("Test 4: \nTry to double free \n");
	void *x = malloc(1);

	free(x); 
	free(x);

	printf("Is memory leaky: %d\n\n", isMemoryLeaky());
}

void t5(){
	printf("Test 5: \nTry to free a pointer that has not been allocated using malloc()\n");
	int x = 5;
	free(&x);

	printf("Is memory leaky: %d\n\n", isMemoryLeaky());
}

void t6(){

	// show that double free only works sometimes
	printf("Test 6: \nShow that double free only works sometimes \n");
	void *x = malloc(1);
	void *y = malloc(1);
	void *z = y;
	free(x);
	free(y);
	free(z);

	printf("Is memory leaky: %d\n\n", isMemoryLeaky());


}

void t7(){
	// allocate greater than the maximum size of heap, should return an error
	printf("Test 7: \nTry to allocate greater than the maximum size of heap \n");

	void *ptr = malloc(MEMSIZE*sizeof(double) + 1);

	if( ptr != NULL){
		printf("Successfully allocated memory for ptr\n");
	}

	printf("Is memory leaky: %d\n\n", isMemoryLeaky());

}

