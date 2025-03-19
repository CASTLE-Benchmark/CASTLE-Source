/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-416-6.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-416-6.c -o CASTLE-416-6
vulnerable: true
description: Writing to memory that has been freed
cwe: 416
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int* allocateArray(int size) {
    int* buffer = (int*) malloc(size * sizeof(int));

    if (buffer == NULL) {
        printf("Failed to allocate memory.\n");
        return NULL;
    }
    return buffer;
}

void cleanup(int* ptr, int size) {
    for (int i = 0; i < size; i++) {
        ptr[i] = 0;
    }
    free(ptr);
}

int main() {
    int size;
    printf("Enter the size of the array: ");
    scanf("%9d", &size);

    int* array = allocateArray(5);
    if (array == NULL) {
        exit(1);
    }
    cleanup(array, 5);

    array[0] = size; // {!LINE}
    printf("First element of the array: %d\n", size);
    return 0;
}
