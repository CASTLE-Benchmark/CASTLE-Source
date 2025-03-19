/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-253-10.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/92718/versions/1.0.0
version: 1.1
compile: gcc CASTLE-253-10.c -o CASTLE-253-10
vulnerable: false
description: Fgets is null checked correctly.
cwe: 253
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void echo()
{
    char dataBuffer[100] = "";
    char * data = dataBuffer;
    printf("Please enter a string: ");

    if (fgets(data, 100, stdin) == NULL)
    {
        printf("fgets failed!\n");
        exit(1);
    }
    printf("%s\n", data);
}

int main()
{
    echo();
    return 0;
}