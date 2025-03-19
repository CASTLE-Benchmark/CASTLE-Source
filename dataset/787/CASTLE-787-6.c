/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-787-6.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-787-6.c -o CASTLE-787-6 -lm
vulnerable: true
description: Writing outside of array bounds due to wrong array length calculation
cwe: 787
============================================================================
*/

#include <stdio.h>
#include <math.h>

double mySin(int a) {
    return sin(a * 0.01);
}

void fillArray(double* arr, int n, double (*func)(int)) {
    for (int i = 0; i < n; ++i) {
        arr[i] = func(i); // {!LINE}
    }
}

int main() {
    double a[200];

    fillArray(a, sizeof(a) / sizeof(float), mySin); // {!LINE}
    for (int i = 0; i < 200; ++i) {
        printf("%f, ", a[i]);
    }
    printf("\n");

    return 0;
}