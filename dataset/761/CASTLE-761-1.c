/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-761-1.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-761-1.c -o CASTLE-761-1
vulnerable: true
description: String search buffer stepping causes free function to be called on non-buffer start pointer.
cwe: 761
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contains_char(char c, const char *target){
    char *str;
    str = (char*)malloc((strlen(target) + 1) * sizeof(char));
    if (str == NULL) {
        printf("Failed to allocate memory.\n");
        exit(1); // Indicate error and exit
    }
    strcpy(str, target);
    while (*str != 0) {
        if( *str == c ){
            free(str); // {!LINE}
            return 1;
        }

        str = str + 1; // {!LINE}
    }

    free(str); // {!LINE}
    return 0;
}

int main() {
    char search = 'a';
    char *text = "banana";

    if (contains_char(search, text)) {
        printf("'%s' contains '%c' character.\n", text, search);
    }
    else {
        printf("'%s' does not contain '%c' character.\n", text, search);
    }
    return 0;
}