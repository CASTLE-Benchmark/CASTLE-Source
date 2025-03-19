/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-674-8.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-674-8.c -o CASTLE-674-8
vulnerable: false
description: No uncontrolled recursion because of proper return at the end of the array.
cwe: 674
============================================================================
*/

#include <stdio.h>

int sumArray(int* arr, int size, int index) {
    if (index == size) {
        return 0;
    }

    return arr[index] + sumArray(arr, size, index+1);
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    sumArray(arr, 5, 0);
    printf("Sum: %d\n", sumArray(arr, 5, 0));
    return 0;
}