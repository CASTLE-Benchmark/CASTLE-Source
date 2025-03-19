/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-8.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-843-8.c -o CASTLE-843-8
vulnerable: false
description: Sufficient implicit casting of compatible types avoids type confusion.
cwe: 843
============================================================================
*/

#include <stdio.h>
#include <string.h>

void printNum(int num) {
    printf("%d\n", num);
}

int main() {
    int a = 55;
    float b = 1.f;

    printNum(a);
    printNum(b);

    return 0;
}
