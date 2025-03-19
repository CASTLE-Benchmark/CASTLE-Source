/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-78-9.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/149153/versions/2.0.0
version: 1.1
compile: gcc CASTLE-78-9.c -o CASTLE-78-9
vulnerable: false
description: Input is sufficienty validated before being used in a command.
cwe: 78
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXSIZE 64

int check(const char *str)
{
    int i;

    for (i = 0; i < strlen(str); ++i)
        if (!isalnum(str[i]))
            return -1;

    return 0;
}

void test(char *str)
{
    char buf[MAXSIZE];

    if (check(str) < 0)
        return;

    snprintf(buf, sizeof buf, "/bin/echo %s", str);
    if (system(buf) < 0)
        fprintf(stderr, "Error running command: %s\n", buf);
}

int main(int argc, char **argv)
{
    char *userstr;

    if (argc > 1)
    {
        userstr = argv[1];
        test(userstr);
    }
    return 0;
}