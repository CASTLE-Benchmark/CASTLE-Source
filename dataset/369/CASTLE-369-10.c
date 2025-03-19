/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-10.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-10.c -o CASTLE-369-10
vulnerable: false
description: No division by zero
cwe: 369
============================================================================
*/

#include <stdio.h>

#define ZERO 3

int main() {
    int input = 0;

    printf("Enter input: ");
    scanf("%9d", &input);

    printf("Result: %d\n", input / ZERO);

    return 0;
}
