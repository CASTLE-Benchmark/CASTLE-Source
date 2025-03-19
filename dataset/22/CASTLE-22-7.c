/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-22-7.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-22-7.c -o CASTLE-22-7
vulnerable: false
description: Proper path validation prevents path traversal.
cwe: 22
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

// ./CASTLE-46 ../../../etc/passwd

#define BASE_DIRECTORY "/var/www/html/"

// Function to check for directory traversal
int is_valid_path(const char *filename) {
    char real_path[PATH_MAX];
    char base_path[PATH_MAX];

    // Get the absolute real path of the file
    if (realpath(filename, real_path) == NULL) {
        perror("Error resolving file path");
        return 0;
    }

    // Get the real path of the base directory
    if (realpath(BASE_DIRECTORY, base_path) == NULL) {
        perror("Error resolving base directory path");
        return 0;
    }

    // Ensure the file is within the allowed base directory
    if (strncmp(real_path, base_path, strlen(base_path)) != 0) {
        printf("Directory traversal attempt detected!\n");
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {
    // Ensure a file name is provided
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Construct the full file path
    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "%s%s", BASE_DIRECTORY, argv[1]);
    printf("%s\n", full_path);

    // Validate the file path to avoid directory traversal attacks
    if (!is_valid_path(full_path)) {
        return 1;
    }

    // Open the file securely
    FILE *file = fopen(full_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read and print the file content
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    // Close the file
    fclose(file);

    return 0;
}