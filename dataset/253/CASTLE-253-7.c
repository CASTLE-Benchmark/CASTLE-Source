/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-253-7.c
author: Krisztofer Horvat Zoltan
version: 1.1
compile: gcc CASTLE-253-7.c -o CASTLE-253-7
vulnerable: false
description: Correct check of function return value.
cwe: 253
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define IsNegative(x) x == NULL

int main() {
    int* p = (int*)malloc(sizeof(int));
    if (IsNegative(p)) {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    *p = 7;
    printf("Value: %d\n", *p);

    free(p);

    return 0;
}
