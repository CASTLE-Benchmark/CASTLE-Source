/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-416-3.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-416-3.c -o CASTLE-416-3
vulnerable: true
description: Memory is used after being freed, tricked by swap function
cwe: 416
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void square(int* a) {
    int b = *a;
    *a = b * b;
}

void swap(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));

    if (a == NULL || b == NULL) {
        free(a);
        free(b);
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    square(a);
    printf("Result: %d\n", *a);

    *b = *a;
    free(a);

    swap(&a, &b);
    printf("Result: %d\n", *b); // {!LINE}

    free(a);

    return 0;
}
