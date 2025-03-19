/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-125-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-125-1.c -o CASTLE-125-1
vulnerable: true
description: Upper array boundary error violation.
cwe: 125
============================================================================
*/

#include <stdio.h>

int main() {
    int array[5] = {1, 2, 3, 4, 5};

    for(int i=0; i<=5; i++) {
        printf("%d\n", array[i]); // {!LINE}
    }

    return 0;
}
