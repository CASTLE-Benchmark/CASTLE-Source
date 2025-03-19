/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-1.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-1.c -o CASTLE-362-1
vulnerable: true
description: Race condition is not properly handled in cases where mutex lock cannot be acquired.
cwe: 362
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
int sharedResource = 0;

void* threadFunction(void* arg) {
    pthread_mutex_lock(&mutex); // {!LINE}

    // Simulate some work with the shared resource
    int temp = sharedResource;
    printf("Thread %ld: Read sharedResource = %d\n", (long)arg, temp);
    if ((long)arg % 2 == 0) {
        sleep(1); // Simulate a delay
    }
    sharedResource = temp + 1;
    printf("Thread %ld: Updated sharedResource to %d\n", (long)arg, sharedResource);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t threads[5];

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create multiple threads
    for (long i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, threadFunction, (void*)i);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    printf("Final value of sharedResource = %d\n", sharedResource);

    return 0;
}