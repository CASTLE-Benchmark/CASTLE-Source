/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-190-5.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-190-5.c -o CASTLE-190-5
vulnerable: true
description: Integer overflow after set operations.
cwe: 190
============================================================================
*/

#include <stdio.h>
#include <math.h>


int main() {
    int a = 101;
    int b = a*a*a;
    int c = b*2+15000;
    int d = c^15;
    int e = d;

    for (int i = 0; i < 3; i++) {
        e *= e; // {!LINE}
    }

    printf("%d\n", e);

    return 0;
}
