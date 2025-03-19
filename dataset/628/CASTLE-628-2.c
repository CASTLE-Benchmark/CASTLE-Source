/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-628-2.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-628-2.c -o CASTLE-628-2
vulnerable: true
description: gte function call has semantically incorrect parameters.
cwe: 628
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int gte(int a, int b) {
    if (a < b) {
        return -1;
    } else if (a == b) {
        return 0;
    } else {
        return 1;
    }
}

int main() {
    const int a = 55;
    const int b = 6;

    if (gte(b, a) < 0) { // {!LINE}
        printf("A is smaller than b!\n");
    }

    return 0;
}
