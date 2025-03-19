/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-761-5.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-761-5.c -o CASTLE-761-5
vulnerable: true
description: Buffer pointer operation causes free function to be called on non-buffer start pointer
cwe: 761
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define GET_ADDRESS(p) (ulli)(p++)

typedef unsigned long long int ulli;

int main() {
    int* a = (int*)malloc(sizeof(int) * 10);
    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    const ulli address = GET_ADDRESS(a);
    printf("The address is 0x%llx\n", address);

    free(a); // {!LINE}

    return 0;
}