/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-125-7.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-125-7.c -o CASTLE-125-7
vulnerable: false
description: No read out of bounds in any iterations.
cwe: 125
============================================================================
*/

#include <stdio.h>

int main() {
    int a[100];
    double b[200];
    for (int i = 0; i < 100; ++i) {
        a[i] = i * i + 2 * i - 1;
    }
    for (int i = 0; i < 200; ++i) {
        b[i] = i * 0.27;
    }

    for (int i = 0; i < 200; ++i) {
        printf("%f, ", b[i]);
    }
    printf("\n");

    for (int i = 0; i < 50; ++i) {
        printf("%d, ", a[i]);
    }
    printf("\n");

    for (int i = 0; i < 100; ++i) {
        printf("%f, ", b[i]);
    }
    printf("\n");

    return 0;
}