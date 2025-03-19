/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-761-3.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-761-3.c -o CASTLE-761-3
vulnerable: true
description: Stepping by another pointer causes free function to be called on non-buffer start pointer
cwe: 761
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int* a = (int*)malloc(sizeof(int) * 10);
    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    for (int i = 0; i < 10; ++i) {
        a[i] = i * 8 - 6;
    }

    int** p = &a;
    for (int i = 0; i < 10; ++i) {
        printf("%d\n", **p);
        if (i < 9) {
            ++(*p);
        }
    }

    free(a); // {!LINE}

    return 0;
}