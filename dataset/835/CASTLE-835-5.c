/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-835-5.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-835-5.c -o CASTLE-835-5
vulnerable: true
description: Infinite loop because of incorrect iteration
cwe: 835
============================================================================
*/

#include <stdio.h>

void printIntLine(int i)
{
    printf("%d\n", i);
}

int loop_result_getter()
{
    int i = 0;
    while (i >= 0) // {!LINE}
    {
        printIntLine(i);
        i = (i + 1) % 256; // {!LINE}
    }

    return i;
}

int main() {
    int res = loop_result_getter();
    printf("Result: %d\n", res);
    return 0;
}