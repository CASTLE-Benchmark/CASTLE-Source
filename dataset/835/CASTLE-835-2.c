/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-2.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-2.c -o CASTLE-835-2
vulnerable: true
description: Infinite loop because of a missing increment
cwe: 835
============================================================================
*/

#include <stdio.h>

int main() {
    int a = 0;
    int b = 1;
    while (a < 10) { // {!LINE}
        printf("pong\n");
        b++;
    }
    return 0;
}