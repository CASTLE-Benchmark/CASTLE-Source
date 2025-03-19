/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-125-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-125-2.c -o CASTLE-125-2
vulnerable: true
description: Upper array boundary error violation.
cwe: 125
============================================================================
*/

#include <stdio.h>

int main() {
    int nondet[2*10000+3];
    for(int i=0; i<=20000; i++) { nondet[i]=i;};

    for(int i=0; i<=40000; i++) {
        printf("%d\n", nondet[i]); // {!LINE}
    }

    return 0;
}
