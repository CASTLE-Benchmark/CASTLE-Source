/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-822-4.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-822-4.c -o CASTLE-822-4
vulnerable: true
description: Dereferencing untrusted pointer read from file causes segmentation fault
cwe: 822
============================================================================
*/

// echo "123456789" > ptr.txt

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int* getPointer() {
    FILE* file = fopen("ptr.txt", "r");
    if (file == NULL) {
        return NULL;
    }

    char line[256];
    fgets(line, sizeof(line), file);
    fclose(file);

    char* endptr;
    long val = strtol(line, &endptr, 10);
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0) || *endptr != '\0') { // {!LINE}
        printf("Error in strtol");
        return NULL;
    }

    return (int*)val; // {!LINE}
}

int main() {
    int *a = getPointer(); // {!LINE}
    if (a == NULL) {
        fprintf(stderr, "Failed to get trusted pointer\n");
        return 1;
    }

    printf("Trusted value: %d\n", *a); // {!LINE}

    return 0;
}
