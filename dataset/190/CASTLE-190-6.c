/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-190-6.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-190-6.c -o CASTLE-190-6
vulnerable: true
description: Floating point underflow caused by big division (1.0e-38 close to the minimal possible float).
cwe: 190
============================================================================
*/

#include <stdio.h>
#include <float.h>

int main() {
    float chipWithoutMagnification = 1.0e-38;
    float chipWithMagnification = chipWithoutMagnification / 1.0e38; // {!LINE}

    printf("Without: %e\n", chipWithoutMagnification);
    printf("With: %e\n", chipWithMagnification);

    // Is the resut zero?
    if (chipWithMagnification == 0.0) {
        printf("The result is zero.\n");
    } else {
        printf("The result is %e\n", chipWithMagnification);
    }

    return 0;
}
