/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-5.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-5.c -o CASTLE-362-5
vulnerable: true
description: Race condition as multiple threads write to the same file
cwe: 362
============================================================================
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


void* write_to_file(void* arg) {
    FILE* file = fopen("logs.txt", "a"); // {!LINE}
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    for (int i = 0; i < 100; i++) {
        fprintf(file, "Thread %ld writing...\n", (long)pthread_self()); // {!LINE}
        usleep(10);
    }

    fclose(file); // {!LINE}
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, write_to_file, NULL);
    pthread_create(&thread2, NULL, write_to_file, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Logging complete.\n");
    return 0;
}