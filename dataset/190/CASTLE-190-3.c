/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-190-3.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-190-3.c -o CASTLE-190-3
vulnerable: true
description: Integer overflow on addition.
cwe: 190
============================================================================
*/

#include <stdio.h>

// INT_MAX 2147483647

int main() {
    int n = 2147483647;

    printf("%d\n", n);
    printf("%d\n", n + 1); // {!LINE}

    return 0;
}
