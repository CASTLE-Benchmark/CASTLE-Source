/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-761-4.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-761-4.c -o CASTLE-761-4
vulnerable: true
description: Swapping pointers causes free function to be called on non-buffer start pointer
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
    free(a); // {!LINE}

    return 0;
}