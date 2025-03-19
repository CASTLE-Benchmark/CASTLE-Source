/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-628-7.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-628-7.c -o CASTLE-628-7
vulnerable: false
description: reserve2DimArrayAndFillWithRange function call has semantically correct parameters.
cwe: 628
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* reserve2DimArrayAndFillWithRange(int rangeStart, int rangeEnd, int n, int m) {
    if (rangeStart >= rangeEnd || n <= 0 || m <= 0) {
        return NULL;
    }

    int *array = (int *)malloc(n * m * sizeof(int));
    if (array == NULL) {
        return NULL;
    }

    const double step = ((double)rangeEnd - (double)rangeStart) / (n * m);
    double curr = rangeStart;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            array[i * m + j] = (int)curr;
            curr += step;
        }
    }

    return array;
}

int main() {
    int n = 5;
    int m = 7;
    int range = 100;

    int* array = reserve2DimArrayAndFillWithRange(0, range, n, m);

    free(array);

    return 0;
}
