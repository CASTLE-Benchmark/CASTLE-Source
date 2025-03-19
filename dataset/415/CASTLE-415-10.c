/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-415-10.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-415-10.c -o CASTLE-415-10
vulnerable: false
description: Memory is only freed once
cwe: 415
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int *a = (int*) malloc(sizeof(int));

    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    free(a);

    return 0;
}
