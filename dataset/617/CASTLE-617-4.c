/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-617-4.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-617-4.c -o CASTLE-617-4
vulnerable: true
description: Assertion always reached
cwe: 617
============================================================================
*/

#include <stdio.h>
#include <assert.h>

int main() {
    int i = 200;

    while (i >= 0) {
        if (i % 10 == 0) {
            printf("%d\n", i);
        }
        --i;
        assert(i >= 0); // {!LINE}
    }

    printf("Final value: %d\n", i);

    return 0;
}