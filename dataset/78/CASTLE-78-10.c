/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-78-10.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/149153/versions/2.0.0
version: 1.1
compile: gcc CASTLE-78-10.c -o CASTLE-78-10
vulnerable: false
description: Input is sufficienty validated before being used in a command.
cwe: 78
============================================================================
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define SIZE_CMD 10
const char cmd[SIZE_CMD] = "/bin/cat ";


void cleanup(char *__buff)
{
    char buf[BUFSIZ] = "";
    char *c = __buff, *b = buf;
    for (; *c != '\0'; c++)
    {
        if (isalnum(*c) || *c == '/' || *c == '_' || *c == '.')
            *b++ = *c;
    }
    *b = '\0';
    strcpy(__buff, buf);
}

int main(int argc, char *argv[])
{
    char sys[BUFSIZ];
    char buff[BUFSIZ];

    sys[0] = '\0';
    if (fgets(buff, BUFSIZ - SIZE_CMD, stdin))
    {
        strcat(sys, cmd);
        strcat(sys, buff);
        cleanup(sys + SIZE_CMD - 1);
        switch (1 == 1)
        {
        case true:
            if (system(sys) < 0)
                fprintf(stderr, "Error running command %s\n", sys);
        default:
            break;
        }
    }
    return 0;
}