#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include "mymalloc.h"
void setSize(void* ptr, int size);
void setIfAllocated(void* ptr, int isAllocated);
int getSize(void* ptr);
int getIfAllocated(char *ptr);

#define MEMSIZE 512
#define ROUND(n) ((n + 7) & (-8))
static double memory[MEMSIZE];

void *mymalloc(size_t size, char *file, int line){
    // if user tries to malloc 0 or less bytes
    if(size<=0){
        fprintf(stderr, "Cannot allocate 0 bytes, error in malloc %s at line %d\n", file, line); 
        return NULL;
    }
    // if user tries to malloc a size larger than the maximum size
	if (size > MEMSIZE*sizeof(double) - 8){
		fprintf(stderr, "Size too large to malloc, error in malloc %s at line %d\n", file, line);
        return NULL;
	}
    // round the size to a multiple of 8
    size = ROUND(size);

    char *ptr = (char*) memory+8;
    void *actualmem = NULL;
    char *endOfArray = (char*) (memory) + MEMSIZE*sizeof(double);

    // check if memory if uninitialized; if it is, malloc occurs at beginning of memory
    if(getIfAllocated((char*) memory+8) == 0){ 
        setSize(ptr, size);
        setIfAllocated(ptr, 1);
        actualmem = ptr;
        ptr = ptr+size;
        // if u want to allocate the maximum heap size, you dont want to create a free block at end
        if((ptr)< ((char*) (memory) + MEMSIZE*sizeof(double))){
        	setIfAllocated(ptr+8, 0);
			setSize(ptr+8, MEMSIZE*sizeof(double)-size-16);
		
        }
        return actualmem;
        
    }
	
    // loop through the array you find a portion of the heap that is large enough for our data
    while(ptr < endOfArray){
		int sizeOfBlock = getSize(ptr);
        // ensure that the block is large enough and the block is free
        if((getIfAllocated(ptr) == 0 || getIfAllocated(ptr) == 2) && sizeOfBlock > (size)){
			setSize(ptr, size);
			setIfAllocated(ptr, 1);
			actualmem = ptr;
			ptr = ptr+size+8;
       		setIfAllocated(ptr, 0);
			setSize(ptr, sizeOfBlock - size - 8);
			return actualmem;
        }
        // if the size is exactly the same as what we need, do not add a new header at the end 
		else if ((getIfAllocated(ptr) == 0 || getIfAllocated(ptr) == 2) && sizeOfBlock == (size)){
			setSize(ptr, size);
			setIfAllocated(ptr, 1);
			return ptr;
		}
        // continue iteration
		else if (getIfAllocated(ptr) == 1 || sizeOfBlock < size){
			ptr = ptr + sizeOfBlock + 8;
		}

		else{
			fprintf(stderr, "No free space available, error in malloc %s at line %d\n", file, line);
			return NULL;
		}
    }
	fprintf(stderr, "No space to allocate, error in malloc %s at line %d\n", file, line);
    return NULL;

}

void myfree(void *ptr, char *file, int line){
    char *endOfArray = (char*) (memory) + MEMSIZE*sizeof(double);
    char *prev = (char*) (memory) + 8;
    char* temp = NULL;
    if ((char*) ptr > endOfArray || (char*) ptr < (char*) memory){ //if ptr if out of bounds of heap
        fprintf(stderr, "Invalid free, pointer out of bounds, error in file %s at line %d\n", file, line);
        return;
    }
    else if(getIfAllocated((char*) memory + 8) == 0 && getSize((char*) memory + 8) == 0){ //memory uninitialized 
        fprintf(stderr, "Memory is uninitialized, nothing to free, error in file %s at line %d\n", file, line);
        return;
    }
    // iterate until you either reach ptr or cross ptr
    while (prev < (char*) ptr){
        temp = prev;
        prev = prev + getSize(prev) + 8;
        //temp variable that holds onto prev for coalescing's sake
    }
    if (prev == ptr){
        if (getIfAllocated(ptr) == 2){
            // check if getIfAllocated is 1
            // if 2, then return error -- double free
            fprintf(stderr, "Pointer has already been freed (double free), error in file %s at line %d\n", file, line);
            return;
        }
        else{
            // otherwise setIfAllocated to 2 - freed pointer
            setIfAllocated(ptr, 2);
            char *future = ptr + getSize(ptr) + 8;
            // different checks to check validity of pointers and which adjacent blocks are free 
            if(temp == NULL){ 
                if (future < endOfArray && (getIfAllocated(future) == 0 || getIfAllocated(future) == 2 )){
                    setSize(ptr, getSize(ptr) + 8 + getSize(future));
                }
            }
            else{
            // coalesce
                if(getIfAllocated(temp) == 0 || getIfAllocated(temp) == 2){
                    if((future < endOfArray) && (getIfAllocated(future) == 0 || getIfAllocated(future) == 2)){
                        // temp is free, current free, future free 
                        setSize(temp, getSize(temp) + 8 + getSize(ptr) + 8 + getSize(future));
                    }
                    else{
                        setSize(temp, getSize(temp) + 8 + getSize(ptr));
                        // temp is free, current free, future blocked
                    }
                }
                else{
                    if((future < endOfArray) && (getIfAllocated(future) == 0 || getIfAllocated(future) == 2)){
                        setSize(ptr, getSize(ptr) + 8 + getSize(future));
                        //temp blocked, current free, future free
                    }
                }
            }

        }
    }
    else if (prev != ptr){ // invalid ptr
        fprintf(stderr, "Invalid pointer, does not point to start of data block, error in file %s at line %d\n", file, line);
        return;
    }

}

void setSize(void* ptr, int size){
    // set size of the block in metadata
    *((int *) (ptr - 8)) = size;
}

void setIfAllocated(void* ptr, int isAllocated){
    *((int *) (ptr - 4)) = isAllocated;
    // set whether the block is allocated, freed, or never allocated
    // 0 = no allocation, 1 = allocated, 2 = pointer has been freed 
}

int getSize(void* ptr){
    return *((int *) (ptr - 8));
    // get the size of the data block
}

int getIfAllocated(char *ptr){
    return *((int *) (ptr - 4));
    //get the allocation status of the data block
}

int isMemoryLeaky(){ 
// checking if, at the end of the program, the array has one free block containing the max size of memory
// making sure that the memory has no additional allocated pointers at the end of it
    if(((getIfAllocated(((char*) memory) + 8) == 0) || getIfAllocated(((char*) memory) + 8) == 2)
     && (getSize(((char*) memory) + 8) == (MEMSIZE*sizeof(double) - 8))){
		return 0;
    }
	return 1;

}