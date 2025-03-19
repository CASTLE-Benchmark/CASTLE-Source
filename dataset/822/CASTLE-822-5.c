/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-822-5.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-822-5.c -o CASTLE-822-5
vulnerable: true
description: Dereferencing untrusted pointer read from environment variable causes segmentation fault
cwe: 822
============================================================================
*/

// export PTR=123456789

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int* getTrustedPointer() {
    char* ptr = getenv("PTR");
    if (ptr == NULL) {
        return NULL;
    }

    char* endptr;
    long val = strtol(ptr, &endptr, 10);
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0) || *endptr != '\0') { // {!LINE}
        printf("Error in strtol");
        return NULL;
    }

    return (int*)val; // {!LINE}
}

int main() {
    int *a = getTrustedPointer(); // {!LINE}
    if (a == NULL) {
        fprintf(stderr, "Failed to get trusted pointer\n");
        return 1;
    }

    printf("Trusted value: %d\n", *a); // {!LINE}

    return 0;
}
