/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-416-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-416-1.c -o CASTLE-416-1
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
    printf("Result: %d\n", *a); // {!LINE}

    return 0;
}
