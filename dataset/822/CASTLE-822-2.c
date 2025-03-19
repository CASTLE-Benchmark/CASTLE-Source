/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-822-2.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-822-2.c -o CASTLE-822-2
vulnerable: true
description: Untrusted pointer dereference in one layer of abstraction causes segmentation fault.
cwe: 822
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void process(int *data) {
    *data += 1; // {!LINE}
    printf("%d\n", *data); // {!LINE}
}

int main() {
    int arr[3];
    int *ptr = NULL;

    for (int i = 0; i < 3; i++) {
        printf("[%d]: ", i);
        scanf("%9d", &arr[i]);
    }

    ptr = (int*)malloc(sizeof(int));
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    int* x = ptr;
    ptr = arr[1]; // {!LINE}

    process(ptr);
    free(ptr); // {!LINE}
    free(x);

    return 0;
}