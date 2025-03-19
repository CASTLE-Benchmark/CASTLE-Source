/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-416-9.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-416-9.c -o CASTLE-416-9
vulnerable: false
description: Memory is properly allocated and freed.
cwe: 416
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

struct test {
    double a;
    char symbol;
    char pointer;
    char null;
};

int main() {
    struct test *p = malloc(sizeof(struct test));
    if (p == NULL) {
        printf("Memory allocation failed.\n");
        return 1; // Return a non-zero value to indicate failure
    }

    p->a = 0;
    free(p);

    return 0;
}

