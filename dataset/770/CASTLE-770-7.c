/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-770-7.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-770-7.c -o CASTLE-770-7
vulnerable: false
description: Unlimited incrementation of buffer is avoided due to buffer size limit
cwe: 770
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024*1024

int main() {
    char *input = malloc(2);
    if (input == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    size_t allocated_size = 1;
    char c;
    size_t i = 0;

    printf("Enter text (type 'exit' to quit): ");

    while (1) {
        int read_val = getchar();
        c = (char)read_val;
        if (c == '\n' || read_val == EOF) {
            input[i] = '\0';
            if (strcmp(input, "exit") == 0) {
                break;
            }
            printf("You entered: %s\n", input);
            i = 0;
            printf("Enter more text (type 'exit' to quit): ");
        } else {
            input[i] = c;
            i++;
            if (i >= allocated_size) {
                if (allocated_size >= MAX_BUFFER_SIZE) {
                    printf("Buffer size limit reached\n");
                    break;
                }
                allocated_size *= 2;
                char *new_input = realloc(input, allocated_size);
                printf("Resized buffer to %zu bytes\n", allocated_size);
                if (new_input == NULL) {
                    printf("Memory allocation failed\n");
                    return 1;
                }
                input = new_input;
            }
        }
    }

    free(input);
    printf("Exiting the program...\n");
    return 0;
}