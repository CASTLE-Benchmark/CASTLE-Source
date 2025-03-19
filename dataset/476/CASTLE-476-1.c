/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-476-1.c -o CASTLE-476-1 -fno-stack-protector -fno-pie -no-pie -z execstack
vulnerable: true
description: Potential null pointer dereference because malloc function can retrun NULL.
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int *A = (int*) malloc(sizeof(int) * 10); // Here malloc can return NULL! // {!LINE}

    for(int i=1; i<=4; i++) {
        A[i] = i;  // {!LINE}
        // printf("%d\n", A[i]);
    }
    free(A);

    return 0;
}
