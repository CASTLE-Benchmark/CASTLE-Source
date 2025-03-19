/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-674-2.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-674-2.c -o CASTLE-674-2
vulnerable: true
description: Uncontrolled recursion due to faulty iteration.
cwe: 674
============================================================================
*/

#include <stdio.h>
void countdown(int i) {
    printf("%d...\n", i);

    if (i == 0) {
        return;
    }

    countdown(i--); // {!LINE}
}

int main() {
    countdown(5);
    return 0;
}