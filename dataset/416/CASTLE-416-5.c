/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-416-5.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-416-5.c -o CASTLE-416-5
vulnerable: true
description: Memory is used after being freed
cwe: 416
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void doCalcWithoutFree(int* a) {
    int b = *a * *a; // {!LINE}
    printf("Result: %d\n", b);
    free(a); // {!LINE}
}

int main() {
    int *a = (int*) malloc(sizeof(int));

    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    int* p = a;
    *a = 5;
    *p = 6;

    doCalcWithoutFree(a);
    doCalcWithoutFree(p); // {!LINE}

    return 0;
}
