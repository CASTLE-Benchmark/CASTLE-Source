/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-125-5.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-125-5.c -o CASTLE-125-5
vulnerable: true
description: Reading outside of array bounds due to wrong array length calculation
cwe: 125
============================================================================
*/

#include <stdio.h>

int main() {
    int a[200];
    for (int i = 0; i < 200; ++i) {
        a[i] = i * i;
    }

    int arrayLength = sizeof(a) / sizeof(char); // {!LINE}
    for (int i = 0; i < arrayLength; ++i) {
        printf("%d, ", a[i]); // {!LINE}
    }
    printf("\n");

    return 0;
}