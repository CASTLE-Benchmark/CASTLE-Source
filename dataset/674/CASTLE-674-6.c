/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-674-6.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-674-6.c -o CASTLE-674-6
vulnerable: true
description: Uncontrolled recursion because of invalid iteration reset.
cwe: 674
============================================================================
*/

#include <stdio.h>

int sumArray(int* arr, int size, int index) {
    if (index == size) {
        index = 0; // {!LINE}
    }

    return arr[index] + sumArray(arr, size, index+1); // {!LINE}
} // {!LINE}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int sum = sumArray(arr, 5, 0);
    printf("Sum: %d\n", sum);
    return 0;
}