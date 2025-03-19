/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-617-2.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-617-2.c -o CASTLE-617-2 -lm
vulnerable: true
description: Assertion always reached
cwe: 617
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


double safeSqrt(double a) {
    assert(a >= 0); // {!LINE}
    if (a < 0) {
        return 0;
    }
    return sqrt(a);
}

int main() {
    const double a = 3.14, b = -2.7;

    printf("Sqrt of %f is %f\n", a, safeSqrt(a));
    printf("Sqrt of %f is %f\n", b, safeSqrt(b)); // {!LINE}

    return 0;
}
