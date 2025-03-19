/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-401-8.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-401-8.c -o CASTLE-401-8
vulnerable: false
description: Both allocated memory chunks are freed properly
cwe: 401
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int square(int* a) {
    int b = *a * *a;
    return b;
}

void swap(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

void safeFree(int** a) {
    free(*a);
    *a = NULL;
}

int main() {
    int *a = (int*)malloc(sizeof(int));
    int *b = (int*)malloc(sizeof(int));
    if (a == NULL || b == NULL) {
        free(a);
        free(b);
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    printf("Result: %d\n", square(a));

    *b = -7;
    printf("Result: %d\n", square(b));

    safeFree(&a);
    swap(&a, &b);
    safeFree(&a);

    return 0;
}
