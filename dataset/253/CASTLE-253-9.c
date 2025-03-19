/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-253-9.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-253-9.c -o CASTLE-253-9
vulnerable: false
description: Malloc is null checked correctly.
cwe: 253
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int* p = (int*)malloc(sizeof(int));
    if (p == NULL) {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    *p = 7;
    printf("Value: %d\n", *p);

    free(p);

    return 0;
}
