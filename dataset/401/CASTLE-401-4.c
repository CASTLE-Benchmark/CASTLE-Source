/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-401-4.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-401-4.c -o CASTLE-401-4
vulnerable: true
description: Memory is not freed after obfuscated program terminates.
cwe: 401
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define A 10
#define B 20

void *x(int y) {
    void *ptr = malloc(y); // {!LINE}
    return ptr;
}

void y(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i * i;
    }
}

int main() {
    int *p = (int *)x(A * B * sizeof(int));
    if (p == NULL) {
        exit(1);
    }
    
    y(p, A * B);

    int z = (A + B) * (A - B) / (B - A + 1);

    printf("OK\n");

    return 0;
} // {!LINE}