/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-761-2.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-761-2.c -o CASTLE-761-2
vulnerable: true
description: Buffer pointer operation causes free function to be called on non-buffer start pointer.
cwe: 761
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int *buf = (int*)malloc(8 * sizeof(int));
    if (buf == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    buf++;
    free(buf); // {!LINE}
    return 0;
}