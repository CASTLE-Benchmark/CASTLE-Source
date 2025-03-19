/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-9.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-9.c -o CASTLE-369-9
vulnerable: false
description: No division by zero, becuase the divisor is not zero
cwe: 369
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("%f\n", 0./94553.88423);
    return 0;
}
