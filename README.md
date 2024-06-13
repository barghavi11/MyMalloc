# My Malloc

Name: Barghavi Gopinath & Upasana Patel 
NetID: bg586 & up62

                         OUR MINI IMPLEMENTATION OF MALLOC() AND FREE():
                         ----------------------------------------------

mymalloc() is a function that allocates the user-requested size of bytes in the heap and returns a 
void pointer that references that portion of heap memory. 
    - The program checks whether the given size is greater than 0, and less than the maximum size of 
    the heap. If not, program returns an error. 
    - The program then iterates through the array to find a block size that is both a: free and 
    b: is large enough to allocate the user-given size. 
        - If this is found, add a header that holds the size of the data as well as the
        allocation status. 0 is not allocated, 1 is allocated, and 2 refers to a data block
        that has already been freed. 
    - If the memory is uninitialized, the data is added to the beginning of the heap.
    - If it cannot find a size large enough to fit the byte size of the requested data, the program 
    returns an error.

myfree() is a function that, given a pointer that has been allocated in the heap using mymalloc(), 
frees the heap space associated with that pointer. 
    - The program checks if the pointer is in the bounds of the heap, and throws an error otherwise. 
    - If the heap is uninitialized, then an error is returned as the heap is empty. 
    - The program iterates through the heap until it either reaches or crosses the desired pointer. 
        - If the iterator crosses the desired pointer, then an error is thrown, as the 
        pointer is not pointing to the beginning of a datablock. 
    - If the pointer has been reached, ensure that the pointer has not already been freed. 
    - Free the pointer, and check if any combination of datablocks adjacent to the freed
    block are free. 
        - If they are, then coalesce the blocks by setting the first free adjacent datablock's 
        size equal to the size of the other free blocks and headers. 
        
Unlike the real malloc() and free(), mymalloc() and myfree() have error-detecting mechanisms that
report common errors to the user, as described above. 

MY FILES: 
-------

mymalloc.c: file that has the code for mymalloc() and myfree().

memgrind.c: file that has performance/stress tests. 

memtest.c: file that has one correctness test for mymalloc().
 
mytests.c: file that contains several other correctness tests for mymalloc() and myfree().

INFORMATION ABOUT COMPILING:
---------------------------
make - makes the files and dependencies
./memgrind - compiles and runs memgrind tests for mymalloc() and free()
./memtest - compiles and runs memtest test for mymalloc() and free()
./mytests - compiles and runs my correction tests for mymalloc() and free()


MY ADDITIONAL TEST CASES IN MEMGRIND.C:
--------------------------------------
1. Malloc a pointer of 1 byte and immediately free, then malloc a pointer of 2 bytes, free,
4 bytes, free, ..... up until the maximum multiple of 2 that can be allocated in the heap. 
Then, do the same thing in reverse (ie. malloc maximum multiple of 2, then decrement up until
1 byte).

** This was to test whether how efficiently my program is able to perform several continuous 
malloc() and free() operations all at once. 

2. Create an array of character pointers, and allocate every other alternating character pointer 
in the heap. Then, allocate the remainder of the pointers. After this, free the pointers in the 
same manner in which they were allocated. 

** This test was to check how long heap and memory access takes in my program.


MY CORRECTNESS TEST CASES: 
-------------------------
Test 1:
- malloc a pointer that is the maximum size of the heap, and then free the pointer. 
- This should return no errors. 

Test 2: 
- Try to malloc 0 bytes.
- This should return an error.

Test 3: 
- Try to free a pointer that points to the middle of a data block. 
- This should return an error. 

Test 4: 
- Try to double free a pointer that has been allocated using malloc().
- This should return an error. 

Test 5: 
- Try to free a pointer that has not been allocated using malloc(). 
- This should return an error.

Test 6: 
- This test is to show that my double free does not work for all cases. 
- In the case where a free block gets coalesced with a block before it, my program cannot 
detect that the pointer had been previously freed. 
- This should return an error. 

Test 7: 
- Try to allocate greater than the maximum size of the heap. 
- This should return a error.


** Every one of my tests calls isMemoryLeaky() to check if, at the end of the program, 
the array has one free block containing the max size of memory. This is to make sure that the
memory has no additional allocated pointers at the end of it.
    - 0 means there is no leaky memory
    - 1 means there is leaky memory. 

WHAT DID WE LEARN?
----------------
Making our own implementation of malloc() and free() gave us a much more complete understanding 
of how these functions work. We learned how to create a project essentially from scratch and use 
debugging tools to interpret different errors in our code and find creative solutions for them. We 
learned how to collaborate with another teammate for a more intricate project such as this one. 
Finally, we learned how to create unique test cases to test our code, and this also helped us 
identify different errors that we would not have otherwise caught. 
