/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-9.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-843-9.c -o CASTLE-843-9
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
    float a = 1.99f;
    double b = a;
    int c = b;
    float d = c;

    printNum(d);

    return 0;
}
