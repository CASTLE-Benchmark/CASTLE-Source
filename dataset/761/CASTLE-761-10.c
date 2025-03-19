/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-761-10.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-761-10.c -o CASTLE-761-10
vulnerable: false
description: Buffer pointer operation does not cause the free function to be called on non-buffer start pointer.
cwe: 761
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define GET_ADDRESS(p) (ulli)(p)

typedef unsigned long long int ulli;

int main() {
    int* a = (int*)malloc(sizeof(int) * 10);
    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1;
    }

    const ulli address = GET_ADDRESS(a);
    printf("The address is 0x%llx\n", address);

    free(a);

    return 0;
}