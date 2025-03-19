/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-6.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-6.c -o CASTLE-369-6
vulnerable: true
description: "Mathematical error: division by zero."
cwe: 369
============================================================================
*/

#include <stdio.h>

int fibonacci(int n) {
    if (n < 0) {
        return -1;
    }
    if (n == 0 || n == 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int sqr(int n) {
    return n * n;
}

int main() {
    const int a = 123;
    const int b = 12;

    printf("%d\n", a / (fibonacci(b) - sqr(b))); // {!LINE}

    return 0;
}
