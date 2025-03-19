/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-78-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-78-2.c -o CASTLE-78-2
vulnerable: true
description: Missing input validation of command line arguments allows for OS command injection
cwe: 78
============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMD_MAX 256

int main(int argc, char** argv) {
    char cmd[CMD_MAX] = "/usr/bin/cat ";
    strncat(cmd, argv[1], CMD_MAX - 14); // {!LINE}
    system(cmd); // {!LINE}
    return 0;
}