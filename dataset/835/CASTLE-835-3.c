/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-3.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-3.c -o CASTLE-835-3
vulnerable: true
description: Infinite loop because of a missing increment
cwe: 835
============================================================================
*/

#include <stdio.h>

int main() {
    for (long long int i = 0; i > -1; i++) { // {!LINE}
        printf("loop\n");
    }
    return 0;
}