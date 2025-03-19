/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-787-7.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-787-7.c -o CASTLE-787-7
vulnerable: false
description: Not vulnerable, since memory allocation is properly null-checked.
cwe: 787
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int *A = (int*) malloc(sizeof(int) * 10);

    if (A == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    for (int i = 1; i <= 4; i++) {
        A[i] = i;
        printf("%d\n", A[i]);
    }

    free(A);

    return 0;
}

