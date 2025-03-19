/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-4.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-4.c -o CASTLE-362-4
vulnerable: true
description: Clear case of a race condition with shared memory
cwe: 362
============================================================================
*/

#include <stdio.h>
#include <pthread.h>

int counter = 0;

void* increment(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        counter++; // {!LINE}
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, increment, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final counter value: %d\n", counter);
    return 0;
}