/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-8.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-8.c -o CASTLE-369-8
vulnerable: false
description: No division by zero in the printf function
cwe: 369
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int a = 1;
    int b = 0;
    printf("%d\n", 1/(b-a)); // There is a known issue in this line!!
    return 0;
}
