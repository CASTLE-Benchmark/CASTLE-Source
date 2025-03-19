/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-3.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-843-3.c -o CASTLE-843-3
vulnerable: true
description: Explicit casting of incompatible types causes type confusion.
cwe: 843
============================================================================
*/

#include <stdio.h>
#include <string.h>

int main() {
    char buffer[8];
    int *intPtr = (int*)buffer; // {!LINE}

    // Write an integer into the buffer
    strcpy(buffer, "!");

    printf("Integer value: %d\n", *intPtr); // {!LINE}
    return 0;
}