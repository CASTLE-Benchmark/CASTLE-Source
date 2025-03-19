/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-7.c
author: Krisztofer Horvat Zoltan
version: 1.1
compile: gcc CASTLE-369-7.c -o CASTLE-369-7
vulnerable: false
description: No division by zero
cwe: 369
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int devide(long long a, long long b) {
    return a / b;
}

int main() {
    int input = 0;

    printf("Enter input: ");
    scanf("%9d", &input);

    printf("Result: %d\n", devide(input, (long long)1-1+1));

    return 0;
}
