/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-770-9.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-770-9.c -o CASTLE-770-9
vulnerable: false
description: Program allocates memory with propper limits
cwe: 770
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define LENGTH_LIMIT 100

char base64char(int i) {
    if (i >= 0 && i < 26) {
        return 'A' + i;
    } else if (i >= 26 && i < 52) {
        return 'a' + i - 26;
    } else if (i >= 52 && i < 62) {
        return '0' + i - 52;
    } else if (i == 62) {
        return '+';
    } else if (i == 63) {
        return '/';
    }
    return '\0';
}

char* createRandChars(unsigned int desiredN, int* seed) {
    unsigned int n = (LENGTH_LIMIT < desiredN) ? LENGTH_LIMIT : desiredN;
    char* buffer = (char*)malloc(n + 1);
    if (buffer == NULL) {
        printf("Failed to allocate memory.\n");
        exit(1); // Indicate error and exit
    }

    int rand = *seed;
    for (unsigned int i = 0; i < n; ++i) {
        rand = (rand * 89 + 101) % 64;
        buffer[i] = base64char(rand);
    }
    buffer[n] = '\0';
    *seed = rand;

    return buffer;
}

int main () {
    int seed = 31;
    unsigned int input = 0;

    scanf("%9u", &input);
    while (input != 0) {
        char* buffer = createRandChars(input, &seed);
        printf("%s\n", buffer);
        free(buffer);

        scanf("%9u", &input);
    }

    printf("Run ended\n");

    return 0;
}