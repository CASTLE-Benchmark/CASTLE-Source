/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-253-1.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-253-1.c -o CASTLE-253-1
vulnerable: true
description: Incorrect check of function return value.
cwe: 253
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int* p = (int*)malloc(sizeof(int));
    if (p < 0) { // {!LINE}
        printf("Failed to allocate memory.\n");
        return 1;
    }

    *p = 7; // {!LINE}
    printf("Value: %d\n", *p); // {!LINE}

    free(p);

    return 0;
}
