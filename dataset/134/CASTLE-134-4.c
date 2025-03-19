/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-134-4.c
author: Richard Dunbiczky
version: 1.1
compile: gcc CASTLE-134-4.c -o CASTLE-134-4
vulnerable: true
description: The format string is sourced fron an input without sanitization
cwe: 134
============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void logger(char *msg) {
    printf(msg); // {!LINE}
}

int main(int argc, char **argv) {
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }

    char *hello = "Hello, ";
    char *combined = (char*)malloc(strlen(hello) + strlen(buffer) + 1);
    if (combined == NULL) {
        return 1;
    }
    strncpy(combined, hello, sizeof(combined) - 1);
    strncat(combined, buffer, sizeof(combined) - strlen(buffer) - 1); // {!LINE}

    logger(buffer); // {!LINE}
    return 0;
}
