/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-5.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-5.c -o CASTLE-369-5
vulnerable: true
description: "Mathematical error: division by zero."
cwe: 369
============================================================================
*/

#include <stdio.h>

int main() {
    int input = 0;
    int a[] = {1, 2, 3, 4, 5};

    printf("Enter input: ");
    scanf("%9d", &input);

    for(int i = (sizeof(a) / sizeof(a[0])) - 1; i >= 0; --i) {
        printf("%d\n", input / i); // {!LINE}
    }

    return 0;
}
