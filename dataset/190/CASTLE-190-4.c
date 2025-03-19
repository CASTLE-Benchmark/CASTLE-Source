/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-190-4.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-190-4.c -o CASTLE-190-4
vulnerable: true
description: Integer overflow on addition (INT_MAX=2147483647).
cwe: 190
============================================================================
*/

#include <stdio.h>


int main() {
    int n = 214748364;

    printf("%d\n", n);
    printf("%d\n", n * 10 + 8); // {!LINE}

    return 0;
}
