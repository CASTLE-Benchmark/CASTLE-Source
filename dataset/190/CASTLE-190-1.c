/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-190-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-190-1.c -o CASTLE-190-1
vulnerable: true
description: Arithmetic overflow in multiplication.
cwe: 190
============================================================================
*/

#include <stdio.h>

int main() {
    int x=77;
    int y=x*x*x;
    int z=y*y; // {!LINE}
    unsigned int r= z/1000;
    printf("Result %u\n", r);
    return 0;
}
