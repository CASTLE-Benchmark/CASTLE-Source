/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-125-4.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-125-4.c -o CASTLE-125-4
vulnerable: true
description: Reading from negative index of an array
cwe: 125
============================================================================
*/

#include <stdio.h>

#define ARRAY_LEN 200

int main() {
    int a[ARRAY_LEN];
    for (int i = 0; i < ARRAY_LEN; ++i) {
        a[i] = 3 * i + 2;
    }

    int lastInd = ARRAY_LEN - 1;
    for (int i = lastInd; i >= lastInd - ARRAY_LEN; --i) {
        printf("%d, ", a[i]); // {!LINE}
    }
    printf("\n");

    return 0;
}