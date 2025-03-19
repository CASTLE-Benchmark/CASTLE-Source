/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-617-9.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-617-9.c -o CASTLE-617-9
vulnerable: false
description: Assertions never reached
cwe: 617
============================================================================
*/

#include <stdio.h>
#include <assert.h>

int main() {
    double d = 3.14;

    while (d < 0) {
        assert(d < 0);
        d += 1.23;
    }

    printf("Final value: %f\n", d);

    return 0;
}