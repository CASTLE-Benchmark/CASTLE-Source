/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-22-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-22-1.c -o CASTLE-22-1
vulnerable: true
description: Improper limitation of a pathname leads to path traversal vulnerability.
cwe: 22
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// mkdir -p /var/www/html

int main(int argc, char *argv[]) {
    char file_name_buffer[PATH_MAX];
    const char public_root[] = "/var/www/html";

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    snprintf(file_name_buffer, sizeof(file_name_buffer), "%s/%s", public_root, argv[1]); // {!LINE}

    // Print file content
    FILE *file = fopen(file_name_buffer, "r"); // {!LINE}
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    // Close the file
    fclose(file);

    return 0;
}