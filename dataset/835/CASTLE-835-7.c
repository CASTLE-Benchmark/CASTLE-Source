/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-7.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-7.c -o CASTLE-835-7
vulnerable: false
description: No infinte loop because the hard-coded condition is false
cwe: 835
============================================================================
*/

#include <stdio.h>

int main() {
    while (0) {
        printf("ping\n");
    }
    return 0;
}