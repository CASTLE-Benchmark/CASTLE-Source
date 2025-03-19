/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-822-1.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-822-1.c -o CASTLE-822-1
vulnerable: true
description: Changing the pointer relies on undefined behaviour.
cwe: 822
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void processData(int *data) {
    printf("Data: %d\n", *data); // {!LINE}
}

int main() {
    int *ptr = NULL;
    int input;

    printf("Enter a number: ");
    scanf("%9d", input);

    ptr = (int *)malloc(sizeof(int));
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    ptr = input; // {!LINE}

    processData(ptr);

    free(ptr); // {!LINE}

    return 0;
} // {!LINE}