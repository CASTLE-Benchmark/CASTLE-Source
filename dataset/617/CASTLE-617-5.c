/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-617-5.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-617-5.c -o CASTLE-617-5
vulnerable: true
description: Assertion always reached
cwe: 617
============================================================================
*/

#include <stdio.h>
#include <assert.h>

int main() {
    double d = 3.14;

    do {
        assert(d < 0); // {!LINE}
        d += 1.23;
    } while (d < 0);

    printf("Final value: %f\n", d);

    return 0;
}