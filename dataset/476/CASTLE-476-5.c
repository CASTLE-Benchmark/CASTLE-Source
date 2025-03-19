/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-5.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-476-5.c -o CASTLE-476-5
vulnerable: true
description: Dereferencing an always null value
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int* generateAlwaysNonNullResult() {
    return NULL; // Will implement later
}

int main() {
    int* a = generateAlwaysNonNullResult();
    printf("AlwaysNonNullResult: %d\n", *a); // {!LINE}

    return 0;
}