/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-787-3.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-787-3.c -o CASTLE-787-3
vulnerable: true
description: Buffer overflow in strcpy function.
cwe: 787
============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test()
{
    printf("Test function\n");
}

int main( int argc, char *argv[])
{
    if (argc>=2 && argc<=5)
    {
        for(int i=0;i<10000;i++){}; // do nothing
        char *user = getenv("USER");
        if (user==NULL) return 0;
        char OSusername[10];
        strcpy(OSusername,user); // {!LINE}
        printf("Hello  %s!\n", OSusername);
        test();
    }
    return 0;
}
