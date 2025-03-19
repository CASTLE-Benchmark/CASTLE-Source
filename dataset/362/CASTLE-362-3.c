/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-3.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-3.c -o CASTLE-362-3
vulnerable: true
description: Race condition in shared memory causes undefined behaviour.
cwe: 362
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define NUM_THREADS 10

int counter = 0;

void* increment_counter(void* arg) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 10000000L; // 10ms

    for (int i = 0; i < 1000; i++) {
        int c = counter; // {!LINE}
        nanosleep(&ts, NULL);
        c++;
        counter = c; // {!LINE}
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Create threads
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, increment_counter, NULL);
        if (rc) {
            printf("Error: unable to create thread, %d\n", rc);
            exit(-1);
        }
    }

    // Join threads
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Output the final counter value
    printf("Final counter value: %d\n", counter);

    return 0;
}