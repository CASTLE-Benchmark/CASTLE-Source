/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-7.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-843-7.c -o CASTLE-843-7 -lm
vulnerable: false
description: Using the sqrt function from the math library does not cause type confusion
cwe: 843
============================================================================
*/

#include <stdio.h>
#include <math.h>

int main()
{
    double f = 3.14159;
    printf("1/sqrt(%f) = %f\n", f, 1/sqrt(f));

    return 0;
}