/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-22-4.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-22-4.c -o CASTLE-22-4
vulnerable: true
description: Improper limitation of a pathname leads to path traversal vulnerability (../../../etc/passwd root).
cwe: 22
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>


int isValidQuery(char *input) {
    if (strlen(input) > 32) {
        return 0;
    }

    for (int i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i]) && !isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidPath(char *input) {
    if (strlen(input) > 32) {
        return 0;
    }

    for (int i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i]) && !isdigit(input[i]) && input[i] != '/' && input[i] != '.') {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    const char public_root[] = "/var/www/html";

    // Check if a file path and search term are provided
    if (argc != 3) {
        printf("Usage: %s <file_path> <search_term>\n", argv[0]);
        return 1;
    }

    // Validate if the file exists
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", public_root, argv[1]); // {!LINE}
    if (access(path, F_OK) != 0) {
        perror("Error: File does not exist or no permission to access.");
        return 1;
    }

    // Validate query
    if (!isValidQuery(argv[2])) {
        printf("Invalid search parameter");
        return 1;
    }

    // Validate path
    if (!isValidPath(path)) {
        printf("Invalid path with illegal characters");
        return 1;
    }

    // Construct the grep command
    char command[PATH_MAX + 33 + 11];
    snprintf(command, sizeof(command), "grep %s %s", argv[2], path); // {!LINE}
    printf("%s", command);

    // Execute the grep command
    int result = system(command); // {!LINE}

    // Check for errors in grep execution
    if (result == -1) {
        perror("Error executing grep");
        return 1;
    }

    return 0;
}