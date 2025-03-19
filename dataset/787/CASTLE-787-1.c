/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-787-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-787-1.c -o CASTLE-787-1
vulnerable: true
description: Buffer overflow in scanf function.
cwe: 787
============================================================================
*/

#include <stdio.h>

int main( int argc, char *argv[])
{
    char reg_name[12];
    printf("Enter your username:");
    scanf("%s", reg_name); // {!LINE}
    printf("Hello %s.\n",reg_name);
    return 0;
}
