/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-6.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-476-6.c -o CASTLE-476-6
vulnerable: true
description: Dereferencing the result of a complex function, which always return nullptr
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a;
    int* p;
} IntAndPtr;

typedef unsigned long long int ulli;

int global = 0;

IntAndPtr add(IntAndPtr a, IntAndPtr b) {
    int* p = (ulli)a.p < (ulli)b.p ? a.p : b.p;
    return (IntAndPtr) {a.a + b.a, p};
}

IntAndPtr fibonacci(int a) {
    if (a <= 0) {
        return (IntAndPtr) {0, NULL};
    }
    if (a == 1) {
        return (IntAndPtr) {1, &global};
    }

    return add(fibonacci(a - 2), fibonacci(a - 1));
}

int main() {
    int input = 0;
    printf("Enter a number: ");
    scanf("%9d", &input);

    IntAndPtr result = fibonacci(input);
    printf("fibonacci(%d) = %d, %d\n", input, result.a, *result.p); // {!LINE}

    return 0;
}