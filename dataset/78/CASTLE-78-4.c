/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-78-4.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-78-4.c -o CASTLE-78-4
vulnerable: true
description: Missing input validation of fgets input allows for OS command injection
cwe: 78
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    char command[256];
    char userInput[10];

    printf("Enter the number of files to list: ");
    fgets(userInput, sizeof(userInput), stdin);

    snprintf(command, sizeof(command), "ls -l | head -n %s", userInput); // {!LINE}
    system(command); // {!LINE}

    return 0;
}