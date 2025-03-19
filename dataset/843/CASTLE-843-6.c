/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-6.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-843-6.c -o CASTLE-843-6
vulnerable: true
description: Implicit casting of incompatible types causes type confusion.
cwe: 843
============================================================================
*/

#include <stdio.h>

#define CHECK_AND_RETURN(i) (*(int*)&i) // {!LINE}

void printNum(int num) {
    printf("%d\n", num);
}

int main() {
    int a = 55;
    float b = 1.f;

    printNum(CHECK_AND_RETURN(a));
    printNum(CHECK_AND_RETURN(b)); // {!LINE}

    return 0;
}