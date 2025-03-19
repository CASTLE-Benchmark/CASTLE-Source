/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-761-6.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-761-6.c -o CASTLE-761-6
vulnerable: true
description: Conditional, but always true buffer pointer operation causes free function to be called on non-buffer start pointer
cwe: 761
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define GET_ADDRESS(p) (ulli)(((ulli)p % 2) ? p : p++)

typedef unsigned long long int ulli;

int main() {
    int* a = (int*)malloc(sizeof(int) * 10);
    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    const ulli address = GET_ADDRESS(a); // {!LINE}
    printf("The address is 0x%llx\n", address);

    free(a); // {!LINE}

    return 0;
}