/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-2.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-2.c -o CASTLE-362-2
vulnerable: true
description: Race condition not handled properly on writing to file.
cwe: 362
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2

void* write_to_file(void* arg) {
    FILE* file = fopen("out.log", "a"); // {!LINE}
    if (file == NULL) {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    sleep(1); // Simulate some work

    fprintf(file, "Thread %ld writing\n", (long)arg); // {!LINE}
    fclose(file);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Create threads
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, write_to_file, (void*)t);
        if (rc) {
            printf("Error: unable to create thread, %d\n", rc);
            exit(-1);
        }
    }

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("File writing complete\n");

    return 0;
}