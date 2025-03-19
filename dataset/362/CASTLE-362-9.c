/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-9.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-9.c -o CASTLE-362-9
vulnerable: false
description: Race condition avoided by using different memory for each thread
cwe: 362
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int counter[2] = {0, 0};

void* increment(void* arg) {
    int* id = (int*)arg;
    for (int i = 0; i < 1000000; i++) {
        counter[*id]++;
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int* a = malloc(sizeof(int));
    int* b = malloc(sizeof(int));
    if (a == NULL || b == NULL) {
        free(a);
        free(b);
        exit(-1);
    }
    *a = 0;
    *b = 1;

    pthread_create(&thread1, NULL, increment, a);
    pthread_create(&thread2, NULL, increment, b);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final counter values: %d, %d\n", counter[0], counter[1]);
    
    free(a);
    free(b);

    return 0;
}