#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10 /*Define size of the array*/

// Global variables
int unsortedArray[SIZE] = {5, 3, 1, 9, 8, 2, 4, 7, 10, 6};
int sortedArray[SIZE];

// function to sort the array in a thread
void *sortingThread(void *params) { // passing multiple parameters to the thread
    int start = *((int *)params);
    int end = *((int *)params + 1);

    // Regular Bubblesorting
    for (int i = start; i < end - 1; ++i) {
        for (int j = start; j < end - i - 1; ++j) {
            if (unsortedArray[j] > unsortedArray[j + 1]) {
                // Swap elements
                int temp = unsortedArray[j];
                unsortedArray[j] = unsortedArray[j + 1];
                unsortedArray[j + 1] = temp;
            }
        }
    }

    pthread_exit(NULL);
}

// Thread to merge into a single array
void *mergingThread(void *params) {
    // Merging the two sorted sublists into the sortedArray
    int mid = SIZE / 2;
    int i = 0, j = mid, k = 0;

    while (i < mid && j < SIZE) {
        if (unsortedArray[i] < unsortedArray[j]) {
            sortedArray[k++] = unsortedArray[i++];
        } else {
            sortedArray[k++] = unsortedArray[j++];
        }
    }

    while (i < mid) {
        sortedArray[k++] = unsortedArray[i++];
    }

    while (j < SIZE) {
        sortedArray[k++] = unsortedArray[j++];
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t sortThread1, sortThread2, mergeThread;
    /*Arrays containing the start and end indices of unsorted array*/
    int params1[] = {0, SIZE / 2}; 
    int params2[] = {SIZE / 2, SIZE};

    // Create sorting threads
    pthread_create(&sortThread1, NULL, sortingThread, (void *)params1);
    pthread_create(&sortThread2, NULL, sortingThread, (void *)params2);

    // Wait for sorting threads to finish
    pthread_join(sortThread1, NULL);
    pthread_join(sortThread2, NULL);

    // 1. pointer to mergeThread
    // 2. specifies the attributes of the thread, passing NULL means default attributes get used
    // 3. the function to pass to the thread
    // 4. parameters of the function to be passed in this case no parameters.
    pthread_create(&mergeThread, NULL, mergingThread, NULL);

    // Waits for the merging thread to finish
    pthread_join(mergeThread, NULL);

    // Print the resultant array
    printf("Sorted Array: ");
    for (int i = 0; i < SIZE; ++i) {
        printf("%d ", sortedArray[i]);
    }
    printf("\n");

    return 0;
}