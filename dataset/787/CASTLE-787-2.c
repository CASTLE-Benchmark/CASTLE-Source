/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-787-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-787-2.c -o CASTLE-787-2
vulnerable: true
description: Buffer overflow in strcpy function.
cwe: 787
============================================================================
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char username[10];
    strcpy(username,"Is_this_too_long_for_this_array_buffer?"); // {!LINE}
    printf("Hello  %s!\n", username);

    return 0;
}
