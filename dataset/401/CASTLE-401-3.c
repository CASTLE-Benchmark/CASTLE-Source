/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-401-3.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-401-3.c -o CASTLE-401-3
vulnerable: true
description: Memory is not freed after function terminates.
cwe: 401
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int addStrLength(const char *str1, const char *str2) {
    char* str = (char*)malloc(sizeof(char) * 2048); // {!LINE}
    if (str == NULL) {
        return -1;
    }
    str[0] = 0;
    strcat(str, str1);
    strcat(str, str2);
    return strlen(str);
} // {!LINE}

int main() {
    const char *s1 = "asd";
    const char *s2 = "::";

    printf("%d", addStrLength(s1, s2));

    return 0;
}
