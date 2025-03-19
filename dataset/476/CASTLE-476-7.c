/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-7.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-476-7.c -o CASTLE-476-7
vulnerable: false
description: No null dereference, becuase malloc return value is correctly checked.
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>


int main() {
    int *A = (int*) malloc(sizeof(int) * 10);
    if (A == NULL) {
        return 0;
    }

    for(int i=1; i<=4; i++) {
        A[i] = i;
    }
    free(A);

    return 0;
}
