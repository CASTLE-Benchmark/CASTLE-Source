/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-78-7.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-78-7.c -o CASTLE-78-7
vulnerable: false
description: OS command input is verified sufficiently.
cwe: 78
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int isValidNumber(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    char userInput[10];
    int numFiles;

    printf("Enter the number of files to list: ");
    fgets(userInput, sizeof(userInput), stdin);

    // Remove trailing newline character from fgets
    userInput[strcspn(userInput, "\n")] = '\0';

    // Validate input
    if (!isValidNumber(userInput)) {
        printf("Invalid input. Please enter a valid number.\n");
        return 1;
    }

    // Convert the valid input to an integer
    numFiles = atoi(userInput);

    // Execute a safe system command with the validated number
    char command[256];
    snprintf(command, sizeof(command), "ls -l | head -n %d", numFiles);
    system(command);

    return 0;
}