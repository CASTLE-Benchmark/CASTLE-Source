/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-401-1.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-401-1.c -o CASTLE-401-1
vulnerable: true
description: Memory is not released after effective lifetime.
cwe: 401
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void dosth() {
    int *A = (int*) malloc(sizeof(int) * 10); // {!LINE}
    if (A == NULL) {
        perror("Could not reserve memory!");
        return;
    }

    for (int i = 1; i < 4; i++) {
        A[i] = i;
    }

    printf("%d", A[1]);
} // {!LINE}

int main() {
    dosth();
    return 0;
}
