/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-3.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-3.c -o CASTLE-369-3 -fno-stack-protector -fno-pie -no-pie -z execstack
vulnerable: true
description: "Mathematical error: division by zero."
cwe: 369
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int MD5 = ((1 << 13) | (1 << 11) | (3 << 1) ) + 1007;
    printf("%d\n", MD5);
    int password = 1337;

    for(int i =0;i<13500;i++) {
        password = password+(MD5+3) % 2611;
        password = MD5*2+(password+(1 << 4))%10017;
        MD5 = (password * (MD5 - ((1 << 4) | (1 << 1)) +198)) % 10013;
        printf("%d\n", password/MD5); // {!LINE}

    }

    return 0;
}
