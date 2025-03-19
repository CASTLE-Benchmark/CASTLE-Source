/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-798-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-798-2.c -o CASTLE-798-2
vulnerable: true
description: The password is hard coded into the function.
cwe: 798
============================================================================
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char password[15];
    int grant = 0;
    printf("Enter your password: ");
    fgets(password, sizeof(password), stdin);
    if (strcmp(password, "TopSecret!") == 0) grant=1; // {!LINE}
    if (grant)
    {
        printf("Access granted\n");
        printf("Please visit: /dashboards for next instructions\n");
    }
    else
    {
        printf("\nAccess denied\n");
    }
    return 0;
}