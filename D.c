#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define READERS_COUNT 3
#define WRITERS_COUNT 2

sem_t mutex, writerBlock;
int data = 0, readersCount = 0;

void *reader(void *params) { 
    int readerId = *((int *)params);

    while (1) {
        // Starting process for readers
        sem_wait(&mutex);
        readersCount++;
        if (readersCount == 1) {
            sem_wait(&writerBlock); // If the first reader, block writers
        }
        sem_post(&mutex);

        // Reader's main purpose
        printf("Reader %d reads data: %d\n", readerId, data);

        // Ending process for readers
        sem_wait(&mutex);
        readersCount--;
        if (readersCount == 0) {
            sem_post(&writerBlock); // If the last reader, unblock writers
        }
        sem_post(&mutex);

    }

    pthread_exit(NULL);
}

void *writer(void *params) {
    int writerId = *((int *)params);

    while (1) {
        // Starting process for writers
        sem_wait(&writerBlock);

        // Writer writes the data
        data++; // arbitrary numbers
        printf("Writer %d writes data: %d\n", writerId, data);

        // Ending process 
        sem_post(&writerBlock);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t readers[READERS_COUNT], writers[WRITERS_COUNT];
    int readerIds[READERS_COUNT], writerIds[WRITERS_COUNT];

    // Initialize the semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&writerBlock, 0, 1);

    /*Create reader threads*/ 
    for (int i = 0; i < READERS_COUNT; ++i) {
        readerIds[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, (void *)&readerIds[i]);
    }

    /*Create writer threads*/ 
    for (int i = 0; i < WRITERS_COUNT; ++i) {
        writerIds[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, (void *)&writerIds[i]);
    }

    /*Join reader threads*/ 
    for (int i = 0; i < READERS_COUNT; ++i) {
        pthread_join(readers[i], NULL);
    }

    /*Join writer threads*/ 
    for (int i = 0; i < WRITERS_COUNT; ++i) {
        pthread_join(writers[i], NULL);
    }

    /*Destroy the semaphores*/ 
    sem_destroy(&mutex);
    sem_destroy(&writerBlock);

    return 0;
}
