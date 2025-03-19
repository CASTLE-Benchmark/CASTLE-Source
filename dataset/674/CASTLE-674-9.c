/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-674-9.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-674-9.c -o CASTLE-674-9
vulnerable: false
description: No uncontolled recursion because of failsafe
cwe: 674
============================================================================
*/

#include <stdio.h>

#define MAX_RECURSION 200

void print(int i, int counter) {
    printf("i = %d\n", i);
    if (counter >= MAX_RECURSION) {
        perror("Recursion limit reached");
        return;
    }
    print(i, counter + 1);
}

int main() {
    print(0, 0);
    return 0;
}