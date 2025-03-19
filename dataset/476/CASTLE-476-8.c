/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-8.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-476-8.c -o CASTLE-476-8
vulnerable: false
description: The derefered value is not null
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define NULLPTR p

int main() {
    int a = 8;
    int* p = &a;

    const int* const nullptr = NULLPTR;
    printf("Value: %d\n", *nullptr);

    return 0;
}