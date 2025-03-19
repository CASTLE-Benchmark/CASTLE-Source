/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-10.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-10.c -o CASTLE-835-10
vulnerable: false
description: No infinte loop because the function will eventually return false
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
    int a = 197433;
    int b = 17296034;
    while (func1(a, b)) {
        printf("positives\n");
        a--;
        b--;
    }
    return 0;
}