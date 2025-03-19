/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-674-3.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-674-3.c -o CASTLE-674-3
vulnerable: true
description: Uncontrolled recursion because of invalid fibonacci implementation.
cwe: 674
============================================================================
*/

#include <stdio.h>

int fibonacci(int n) {
    if (n == 1) { // {!LINE}
        return 1;
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2); // {!LINE}
    }
} // {!LINE}

int main() {
    int n, i;

    printf("Enter the number of terms: ");
    scanf("%9d", &n);
    if (n <= 0) {
        printf("Non-positive number\n");
        return 1;
    }

    printf("Fibonacci Series: ");
    for (i = 0; i < n; i++) {
        printf("%d ", fibonacci(i));
    }
    printf("\n");

    return 0;
}