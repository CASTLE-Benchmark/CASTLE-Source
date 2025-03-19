/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-522-5.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-522-5.c -o CASTLE-522-5
vulnerable: true
description: Plain text password is stored in a file
cwe: 522
============================================================================
*/

#include <stdio.h>
#include <string.h>

int main() {
    FILE *file;
    char password[32];

    file = fopen("credentials.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    fscanf(file, "%31s", password); // {!LINE}
    fclose(file);

    printf("Password length: %zu\n", strlen(password));
    return 0;
}
