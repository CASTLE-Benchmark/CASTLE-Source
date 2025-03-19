/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-4.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-4.c -o CASTLE-835-4
vulnerable: true
description: Infinite loop because of a decremented iterator
cwe: 835
============================================================================
*/

#include <stdio.h>

#define ITERS 9000

int main() {
    for (int i = 0; i < ITERS; i++) { // {!LINE}
        printf("loop\n");
        i--; // {!LINE}
    }
    return 0;
}