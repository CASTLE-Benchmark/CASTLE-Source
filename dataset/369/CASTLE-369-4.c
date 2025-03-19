/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-369-4.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-369-4.c -o CASTLE-369-4
vulnerable: true
description: "Mathematical error: division by zero."
cwe: 369
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

void stepRand(int* rand) {
    *rand = (*rand + 1223) % 37;
}

int main() {
    int input = 0;
    int seed = 0;

    printf("Enter input and seed: ");
    scanf("%9d", &input);
    scanf("%9d", &seed);

    int rand = seed % 37;

    for(int i = 0; i < 1000; i++) {
        stepRand(&rand);
        printf("Value now: %d\n", input / rand); // {!LINE}
    }

    return 0;
}
