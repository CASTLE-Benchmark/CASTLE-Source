/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-2.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-843-2.c -o CASTLE-843-2
vulnerable: true
description: Implicit casting of incompatible types causes type confusion.
cwe: 843
============================================================================
*/

#include <stdio.h>
#include <string.h>

void printNum(int* num) {
    printf("%d\n", *num); // {!LINE}
}

int main() {
    int a = 55;
    float b = 1.f;

    printNum(&a);
    printNum(&b); // {!LINE}

    return 0;
}