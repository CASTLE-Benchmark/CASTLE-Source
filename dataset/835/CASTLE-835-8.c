/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-8.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-8.c -o CASTLE-835-8
vulnerable: false
description: No infinite loop because the iteration is correct
cwe: 835
============================================================================
*/

#include <stdio.h>

int main() {
    int a = 0;
    int b = 1;
    while (b < 10) {
        printf("pong\n");
        b++;
    }
    return 0;
}