/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-253-4.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/92718/versions/1.0.0
version: 1.1
compile: gcc CASTLE-253-4.c -o CASTLE-253-4
vulnerable: true
description: Incorrect check of snprintf return value. Should check for -1.
cwe: 253
============================================================================
*/

#include <stdio.h>
#include <string.h>

#define SRC_STRING "random string that definitely has no issues"

int main()
{
    char dataBuffer[100] = "";
    char * data = dataBuffer;

    if (snprintf(data, 100 - strlen(SRC_STRING) -1, "%s\n", SRC_STRING) == 0) // {!LINE}
    {
        printf("snprintf failed!");
    }

    printf("%s\n", data);
    return 0;
}