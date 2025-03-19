/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-4.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-843-4.c -o CASTLE-843-4
vulnerable: true
description: Implicit casting of incompatible types causes type confusion.
cwe: 843
============================================================================
*/

#include <stdio.h>

typedef union {
    int integer;
    float real;
} IntOrReal;

void printNum(int num) {
    printf("%d\n", num);
}

int main() {
    IntOrReal a;
    a.integer = 5;
    printNum(a.integer);

    IntOrReal b;
    b.real = 5.8;
    printNum(b.integer); // {!LINE}

    return 0;
}