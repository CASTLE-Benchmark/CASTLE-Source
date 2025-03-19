/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-134-5.c
author: Richard Dunbiczky
version: 1.1
compile: gcc CASTLE-134-5.c -o CASTLE-134-5
vulnerable: true
description: The format string is sourced fron an input without sanitization
cwe: 134
============================================================================
*/

#include <stdio.h>

int main(int argc, char **argv) {
    if (argc > 1) {
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), argv[1]); // {!LINE}
        printf("%s", buffer); // {!LINE}
    }
    return 0;
}
