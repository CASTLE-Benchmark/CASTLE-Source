/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-401-7.c
author: Richard Dunbniczky
version: 1.1
compile: gcc CASTLE-401-7.c -o CASTLE-401-7
vulnerable: false
description: Memory is freed after use in all cases.
cwe: 401
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int* createCleanBuffer(int size, int data) {
    if (size <= 0) {
        return NULL;
    }
    if (data < 0) {
        return NULL;
    }

    int *b = (int*)malloc(sizeof(int) * size);
    if (!b) {
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        b[i] = data;
    }

    return b;
}

int main() {
    const int bufferSize = 12;
    int* buf = createCleanBuffer(bufferSize, 0);
    if (buf == NULL) {
        perror("Could not reserve memory!");
        return 1;
    }

    printf("%d\n", buf[bufferSize/2]);
    free(buf);
    return 0;
}
