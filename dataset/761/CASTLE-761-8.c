/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-761-8.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-761-8.c -o CASTLE-761-8
vulnerable: false
description: The pointer at the start of the buffer is freed correctly
cwe: 761
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void f(int** a, int**b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int* a = (int*)malloc(sizeof(int) * 10);
    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    for (int i = 0; i < 10; ++i) {
        a[i] = i * 6 + 5;
    }

    int* b = a;
    for (int i = 0; i < 10; ++i) {
        printf("%d\n", *b);
        if (i < 9) {
            ++b;
        }
    }

    f(&a, &b);
    free(b);

    return 0;
}