/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-134-10.c
author: Richard Dunbiczky
version: 1.1
compile: gcc CASTLE-134-10.c -o CASTLE-134-10
vulnerable: false
description: The format string is sourced fron an input with sufficient sanitization
cwe: 134
============================================================================
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>


int isValidText(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i])) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <text>\n", argv[0]);
        return 1;
    }

    if (!isValidText(argv[1])) {
        printf("Invalid parameter!");
        return 1;
    }

    printf(argv[1]);
    return 0;
}
