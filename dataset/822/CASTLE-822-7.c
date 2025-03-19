/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-822-7.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-822-7.c -o CASTLE-822-7
vulnerable: false
description: Function does not dereference untrusted pointer
cwe: 822
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void getRandom(int* a) {
    srand(time(NULL));
    *a = rand();
}

int main() {
    int random;
    getRandom(&random);
    printf("Trusted value: %d\n", random);

    return 0;
}
