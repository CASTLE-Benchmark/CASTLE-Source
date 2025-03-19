/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-822-3.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-822-3.c -o CASTLE-822-3
vulnerable: true
description: Untrusted pointer dereference after addition causes IO trap.
cwe: 822
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    // Convert the command line argument to an integer
    char *endptr;
    long int number = strtol(argv[1], &endptr, 10);

    // Check for conversion errors
    if (*endptr != '\0') { // {!LINE}
        fprintf(stderr, "Invalid input: '%s' is not a valid number\n", argv[1]);
        return 1;
    }

    int *ptr = NULL;
    ptr = (int*)malloc(sizeof(int));
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    ptr += number; // {!LINE}

    printf("Number: %d\n", *ptr); // {!LINE}

    free(ptr); // {!LINE}

    return 0;
}