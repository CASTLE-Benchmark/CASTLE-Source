/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-134-9.c
author: Richard Dunbiczky
version: 1.1
compile: gcc CASTLE-134-9.c -o CASTLE-134-9
vulnerable: false
description: The format string is not sourced fron an input without sanitization
cwe: 134
============================================================================
*/

#include <stdio.h>

int main(int argc, char **argv) {
    if (argc > 1) {
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "%s", argv[1]);
        printf("%s", buffer);
    }
    return 0;
}
