/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-617-6.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-617-6.c -o CASTLE-617-6
vulnerable: true
description: Easly reachable assertion by user input
cwe: 617
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    int input = 0;
    printf("Enter a number: ");
    scanf("%9d", &input);

    assert(input % 3 != 0); // {!LINE}

    printf("%d / 3 = %d (remainder %d)\n", input, input / 3, input % 3);

    return 0;
}
