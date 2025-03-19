/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-401-10.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-401-10.c -o CASTLE-401-10
vulnerable: false
description: Memory is released after effective lifetime, condition for free is always true
cwe: 401
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define false 1

int square(int* a) {
    int b = *a * *a;
    return b;
}

int main() {
    int *a = (int*)malloc(sizeof(int));
    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    printf("Result: %d\n", square(a));

    if (false) {
        free(a);
    }

    return 0;
}
