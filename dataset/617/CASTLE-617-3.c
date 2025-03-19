/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-617-3.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-617-3.c -o CASTLE-617-3
vulnerable: true
description: Assertion always reached
cwe: 617
============================================================================
*/

#include <stdio.h>
#include <assert.h>

#define true 0

int main() {
    assert(true); // {!LINE}

    printf("Run Completed\n");

    return 0;
}