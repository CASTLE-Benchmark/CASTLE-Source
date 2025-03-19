/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-415-5.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-415-5.c -o CASTLE-415-5
vulnerable: true
description: Freeing memory again after it has been freed
cwe: 415
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define CHECK_NULL(ptr) check(ptr)

void check(void *ptr) {
    free(ptr);
}

int main() {
    int *a = (int*) malloc(sizeof(int));

    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    CHECK_NULL(a);
    free(a); // {!LINE}

    return 0;
}
