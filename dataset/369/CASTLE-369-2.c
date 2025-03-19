/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-2.c -o CASTLE-369-2 -fno-stack-protector -fno-pie -no-pie -z execstack
vulnerable: true
description: "Mathematical error: division by zero."
cwe: 369
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int nn=(atoi("2")*500)-1000;
    int what= 1000/ (nn); // {!LINE}
    printf("%d\n", what);

    return 0;
}
