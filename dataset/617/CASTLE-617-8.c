/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-617-8.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-617-8.c -o CASTLE-617-8
vulnerable: false
description: Assertions never reached
cwe: 617
============================================================================
*/

#include <stdio.h>
#include <assert.h>

#define true 0
#define false 1

int main() {
    assert(!true);
    assert(false);

    printf("Run Completed\n");

    return 0;
}