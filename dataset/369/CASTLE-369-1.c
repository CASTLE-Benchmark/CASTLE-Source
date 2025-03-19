/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-1.c -o CASTLE-369-1 -fno-stack-protector -fno-pie -no-pie -z execstack
vulnerable: true
description: "Mathematical error: division by zero."
cwe: 369
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int hello= (8003+1) % 26;
    printf("Result is %d",hello);
    int b=atoi("-42");
    printf("Result is %d",b);
    int test= 17/ (hello+20+b); // {!LINE}
    printf("Result is %d",test);
    return 0;
}
