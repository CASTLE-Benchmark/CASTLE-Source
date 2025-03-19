/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-9.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-476-9.c -o CASTLE-476-9
vulnerable: false
description: Cannot dereference null result because the values are never equal
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int* min(int* a, int* b) {
    if (*a < *b) {
        return a;
    }
    if (*b < *a) {
        return b;
    }
    return NULL;
}

int main() {
    int a = 3, b = 5;

    printf("min(%d, %d) = %d\n", a, b, *min(&a, &b));
    printf("min(%d, %d) = %d\n", b, a, *min(&b, &a));

    return 0;
}