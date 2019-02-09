#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int interval;
int nThreads;
int arraySize;
int * array;

void sort(void *);
int min(int, int);
int powerOfTwo(int);
void printArray();

int main() {
    printf("Please enter an array size that is a power of 2: ");
    scanf("%d", &arraySize);
    if(!powerOfTwo(arraySize)) { 
        printf("Please ensure your array size is a power of 2. Exiting...\n");
        exit(EXIT_FAILURE);
    }

    srand(time(0));
    array = malloc(arraySize * sizeof(int));
    for(int i = 0; i < arraySize; ++i) // Allow elements to be as large as 101
        array[i] = rand() % 101;
    printf("\nUnordered Array: "); printArray();

    // The main Merge Sort loop. The loop variable will increment by powers of 2 and
    // determine the interval size (i.e. the number of elements divvied out to each thread).
    for(interval = 2; interval <= arraySize; interval *= 2) {
        nThreads = arraySize/interval;
        pthread_t myThreads[nThreads];

        for(int i = 0; i < nThreads; ++i)
            pthread_create(&myThreads[i], NULL, (void *)sort, (void *)i);
        for(int i = 0; i < nThreads; ++i)
            pthread_join(myThreads[i], NULL);
    }

    printf("\nOrdered Array: "); printArray();

    exit(EXIT_SUCCESS);
}

void sort(void * vArg) {
    int id = (int)vArg;
    int low = interval * id;
    int high = low + interval;
    int mid = (low + high)/2;

    int lIndex = low;
    int rIndex = mid;
    int tIndex = 0;
    int tempArray[interval];

    while(lIndex < mid || rIndex < high) {
        if(lIndex == mid) // Place leftover elements from right half into local array
            tempArray[tIndex++] = array[rIndex++];
        else if(rIndex == high) // Place leftover elements from left half into local array
            tempArray[tIndex++] = array[lIndex++];
        else // Elements from left and right half still need to be compared and organized into local array
            tempArray[tIndex++] = (array[lIndex] == min(array[lIndex], array[rIndex])) ? array[lIndex++] : array[rIndex++];
    }

    tIndex = 0;
    for(int i = low; i < high; ++i) // Overwrite global array elements with correctly ordered local array elements
        array[i] = tempArray[tIndex++];
}

int min(int l, int r) {return (l < r) ? l : r;}
int powerOfTwo(int toCheck) {return ceil(log2(toCheck)) == floor(log2(toCheck));}

void printArray() {
    printf("[");
    for(int i = 0; i < arraySize; ++i) {
        if(i == arraySize - 1)
            printf("%d", array[i]);
        else
            printf("%d,", array[i]);
    }
    printf("]\n");
}
