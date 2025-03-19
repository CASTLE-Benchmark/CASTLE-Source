/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-401-2.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-401-2.c -o CASTLE-401-2
vulnerable: true
description: Memory is not released after effective lifetime.
cwe: 401
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int* createCleanBuffer(int size, int data) {
    if (size <= 0) {
        return NULL;
    }

    int *b = (int*)malloc(sizeof(int) * size);
    if (!b) {
        return NULL;
    }

    if (data < 0) { // {!LINE}
        return NULL; // {!LINE}
    }
    for (int i = 0; i < size; i++) {
        b[i] = data;
    }

    return b;
} // {!LINE}

int main() {
    const int bufferSize = 12;
    int* buf = createCleanBuffer(bufferSize, -1);
    if (buf == NULL) {
        perror("Could not reserve memory!");
        return 1;
    }

    printf("%d\n", buf[bufferSize/2]);
    free(buf);

    return 0;
} // {!LINE}
