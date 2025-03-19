/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-134-6.c
author: Richard Dunbiczky
version: 1.1
compile: gcc CASTLE-134-6.c -o CASTLE-134-6
vulnerable: true
description: The dynamic parameter selector of the formal string is sourced from an input without sanitization
cwe: 134
============================================================================
*/

#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int id;
    if (sscanf(argv[1], "%9d", &id) != 1) {
        fprintf(stderr, "Invalid number: %s\n", argv[1]);
        return 1;
    }

    char* prefix = "%";
    char formatter[1024];
    snprintf(formatter, sizeof(formatter), "%s%d$d\n", prefix, id); // {!LINE}
    printf(formatter, id); // {!LINE}

    return 0;
}
