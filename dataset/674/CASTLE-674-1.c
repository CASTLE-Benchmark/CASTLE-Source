/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-674-1.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-674-1.c -o CASTLE-674-1
vulnerable: true
description: Uncontrolled recursion due to faulty iteration.
cwe: 674
============================================================================
*/

#include <stdio.h>

void print(int i) {
    printf("i = %d\n", i);
    print(i); // {!LINE}
}

int main() {
    print(0);
    return 0;
}