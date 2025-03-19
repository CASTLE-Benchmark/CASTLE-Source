/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-134-2.c
author: Richard Dunbiczky
version: 1.1
compile: gcc CASTLE-134-2.c -o CASTLE-134-2
vulnerable: true
description: The format string is sourced fron an input without sanitization
cwe: 134
============================================================================
*/

#include <stdio.h>

int main(int argc, char **argv) {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }

    printf(buffer); // {!LINE}
    return 0;
}
