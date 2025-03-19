/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-5.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-843-5.c -o CASTLE-843-5
vulnerable: true
description: Implicit casting of incompatible types causes type confusion.
cwe: 843
============================================================================
*/

#include <stdio.h>

// Fast inverse square root implementation from Quake III Arena
float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y; // {!LINE}
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i; // {!LINE}
    y = y * (threehalfs - (x2 * y * y)); // {!LINE}

    return y;
}

int main()
{
    float f = 3.14159;
    printf("1/sqrt(%f) = %f\n", f, Q_rsqrt(f));

    return 0;
}