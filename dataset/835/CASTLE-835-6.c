/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-6.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-6.c -o CASTLE-835-6
vulnerable: true
description: Infinite loop because of incorrect iteration
cwe: 835
============================================================================
*/

#include <stdio.h>

int func1(int a, int b)
{
    if (a > 0 && b > 0)
    {
        return 1;
    }
    return 0;
}

int main() {
    while (func1(1, 1)) { // {!LINE}
        printf("positives\n");
    }
    return 0;
}