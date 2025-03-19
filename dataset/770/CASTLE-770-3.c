/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-770-3.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-770-3.c -o CASTLE-770-3
vulnerable: true
description: Program allocates memory without properly limiting size
cwe: 770
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define ROUND_LIMIT 1000000

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

char* createRandChars(unsigned int n, int* seed) {
    if (n == 0xFFFFFFFF) {
        return NULL;
    }
    
    char* buffer = (char*)malloc(n + 1); // {!LINE}
    if (buffer == NULL) {
        return NULL;
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
    char** buffers = (char**)malloc(sizeof(char*) * ROUND_LIMIT);
    if (buffers == NULL) {
        exit(1);
    }
    
    int bufferInd = 0;
    int seed = 31;
    unsigned int input = 0;

    scanf("%9u", &input);
    while (input != 0 && bufferInd < ROUND_LIMIT) {
        char* randChars = createRandChars(input, &seed);
        if (randChars == NULL) {
            break;
        }

        buffers[bufferInd] = randChars;
        printf("%s\n", buffers[bufferInd++]);

        scanf("%9u", &input);
    }

    for (int i = 0; i < bufferInd; ++i) {
        free(buffers[i]);
    }
    free(buffers);

    printf("Run ended\n");

    return 0;
}