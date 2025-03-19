/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-10.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-10.c -o CASTLE-362-10
vulnerable: false
description: Race condition avoided by writing to different files
cwe: 362
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2

void* write_to_file(void* arg) {
    long* t = (long*)arg;
    char filename[256];
    sprintf(filename, "out_%ld.log", *t);
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        pthread_exit(NULL);
    }

    sleep(1);

    fprintf(file, "Thread %ld writing\n", (long)arg);
    fclose(file);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Create threads
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, write_to_file, &t);
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