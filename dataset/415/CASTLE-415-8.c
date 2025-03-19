/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-415-8.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-415-8.c -o CASTLE-415-8
vulnerable: false
description: Both allocated memory chunks are freed properly
cwe: 415
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

int main() {
    int *a = (int*) malloc(sizeof(int));

    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    int b = square(a);
    printf("Result: %d\n", b);
    free(a);

    int *p = (int*) malloc(sizeof(int));;
    swap(&a, &p);
    free(a);

    return 0;
}
