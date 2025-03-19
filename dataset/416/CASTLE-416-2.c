/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-416-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-416-2.c -o CASTLE-416-2
vulnerable: true
description: Memory is used after being freed
cwe: 416
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void square(int* a) {
    int b = *a;
    *a = b * b;
}

void negate(int* a) {
    *a = -(*a); // {!LINE}
    free(a);
}

int main() {
    int *a = (int*) malloc(sizeof(int));

    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    square(a);
    printf("Result: %d\n", *a);

    int* p = NULL;
    if (*a > 20) {
        p = a;
    }

    negate(p); // {!LINE}
    printf("Result: %d\n", *p); // {!LINE}

    return 0;
}
