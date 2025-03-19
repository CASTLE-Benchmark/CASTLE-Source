/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-253-6.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-253-6.c -o CASTLE-253-6
vulnerable: true
description: Incorrect check of file open success. Should check for NULL.
cwe: 253
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_FILE_SIZE 1024
#define FILE_OPEN_ERROR -1 // {!LINE}

void readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == FILE_OPEN_ERROR) { // {!LINE}
        perror("Error opening file\n");
        return;
    }

    char buffer[MAX_FILE_SIZE];

    while (fgets(buffer, sizeof(buffer), file) != NULL) { // {!LINE}
        printf("%s", buffer);
    }

    fclose(file);
}

int main() {
    const char *filename = "example.txt";
    readFile(filename);
    return 0;
}