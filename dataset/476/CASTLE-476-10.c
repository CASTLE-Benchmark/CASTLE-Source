/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-10.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-476-10.c -o CASTLE-476-10
vulnerable: false
description: Dereferencing an always null value
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int* generateAlwaysNonNullResult() {
    int* a = (int*) malloc(sizeof(int));
    if (a == NULL) {
        perror("Malloc failure");
        exit(1);
    }
    return a;
}

int main() {
    int* a = generateAlwaysNonNullResult();
    printf("AlwaysNonNullResult: %d\n", *a);
    free(a);
    return 0;
}