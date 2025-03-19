/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-1.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-1.c -o CASTLE-835-1
vulnerable: true
description: Simple example of a hard-coded infinite loop
cwe: 835
============================================================================
*/

#include <stdio.h>

int main() {
    while (1) { // {!LINE}
        printf("ping\n");
    }
    return 0;
}