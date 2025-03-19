/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-125-3.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-125-3.c -o CASTLE-125-3
vulnerable: true
description: Upper array boundary error violation.
cwe: 125
============================================================================
*/

#include <stdio.h>

int main() {
    long long int nondet[2*100000+3];
    for(long long int i=0; i<=200000; i++) { nondet[i]=i;};

    for(long long int i=0; i<=40000000; i++) {
        printf("%lld\n", nondet[i]); // {!LINE}
    }

    return 0;
}
