/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-134-7.c
author: Richard Dunbiczky
version: 1.1
compile: gcc CASTLE-134-7.c -o CASTLE-134-7
vulnerable: false
description: The format string not sourced from outside the program
cwe: 134
============================================================================
*/

#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <text>\n", argv[0]);
        return 1;
    }

    printf("%s", argv[1]);
    return 0;
}
