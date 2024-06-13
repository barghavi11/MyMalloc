#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

#define MEMSIZE 4096

void t1();
void t2();
void t3();
void t4();
void t5();

int main(int argc, char** argv) {

    struct timeval starttime;
    struct timeval endtime;

    gettimeofday(&starttime, 0);

    for(int i = 0; i < 50; i++) {
        
        t1();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t1: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);
    
   
    gettimeofday(&starttime, 0);

    for(int i = 0; i < 50; i++) {

        t2();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t2: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);

    gettimeofday(&starttime, 0);

    for(int i = 0; i < 50; i++) {

        t3();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t3: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);

    gettimeofday(&starttime, 0);

    for(int i = 0; i < 50; i++) {

        t4();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t4: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);

    gettimeofday(&starttime, 0);
    
    for(int i = 0; i < 50; i++) {

        t5();

    }

    gettimeofday(&endtime, 0);

    printf("Average run time for t5: %ld seconds %ld microseconds\n", (endtime.tv_sec-starttime.tv_sec)/50, (endtime.tv_usec-starttime.tv_usec)/50);

}

void t1(){ 
    // malloc() and immediately free()
	for (int i = 0; i< 120; i++){
		char *ptr = malloc(1);

		if (ptr == NULL){
			printf("Error, null pointer returned\n");
		}

		free(ptr);
	}
}

void t2(){ //120 1-byte objects, store in array, and free them
	char *arr[120];
	for (int i = 0; i<120; i++){
		arr[i] = malloc(1);
	}
    for (int i = 0; i<120; i++){
        free(arr[i]);
    }
}
void t3(){ 
    // array of 120 pointers, randomly allocate and free until 120 allocations, free remainder
    char *arr[120];
    int allocations = 0;

    while (allocations < 120){
        if(rand() % 2 == 0 || allocations == 0){
            arr[allocations] = malloc(1);
            allocations++;
        }
        else{
            allocations--;
            free(arr[allocations]);
        }
    }
    allocations = 0; 
    while (allocations<120){
        free(arr[allocations]);
        allocations++;
    }
    
}

void t4(){
    int i = 1;
    while(i<MEMSIZE-8){
        // malloc a pointer of i bytes 
        void* ptr = malloc(i); 
        // free the pointer 
        free(ptr);
        // multiply i by 2 
        i = i * 2;
        // essentially, malloc and free pointers of size 1 byte, 2 bytes, 4 bytes..., 2^n bytes 
        // as long as 2^n ≤ MEMSIZE - 8
    }
    // do the same thing in reverse order

    i = MEMSIZE-8;
    while(i > 0){
        // malloc a pointer of i bytes 
        void* ptr = malloc(i); 

        // free the pointer 
        free(ptr);
        // divide i by 2 
        i = i/2;
    }


}

void t5(){ 
    char *arr[256];
    // allocate every other byte of the array
    for (int i = 0; i<256; i+=2){
        arr[i] = malloc(1);
    }
    // allocate the remaining bytes 
    for (int i = 1; i<256; i+=2){
        arr[i] = malloc(1);
    }

    // free every other byte of the array
    for (int i = 0; i<256; i+=2){
        free(arr[i]);
    }

    // free the remaining bytes
    for (int i = 1; i<256; i+=2){
        free(arr[i]);
    }
}