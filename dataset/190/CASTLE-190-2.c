/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-190-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-190-2.c -o CASTLE-190-2
vulnerable: true
description: Integer overflow in atoi function.
cwe: 190
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    long long int APPLE = atoi("3000000000")+1; // {!LINE}
    printf("%lld\n", APPLE);

    return 0;
}
