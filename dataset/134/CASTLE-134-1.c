/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-134-1.c
author: Richard Dunbiczky
version: 1.1
compile: gcc CASTLE-134-1.c -o CASTLE-134-1
vulnerable: true
description: The format string is sourced fron an input without sanitization
cwe: 134
============================================================================
*/

#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <text>\n", argv[0]);
        return 1;
    }

    printf(argv[1]); // {!LINE}
    return 0;
}
