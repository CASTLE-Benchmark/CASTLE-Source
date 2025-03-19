/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-674-5.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-674-5.c -o CASTLE-674-5
vulnerable: true
description: Uncontrolled recursion because of invalid recursive iteration
cwe: 674
============================================================================
*/

#include <stdio.h>

int recursivePrintArray(int* arr, int size, int index) {
    if (index == size) {
        return 0;
    }

    printf("%d\n", arr[index]);
    return recursivePrintArray(arr, size, (index+1) % size); // {!LINE}
} // {!LINE}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int ret = recursivePrintArray(arr, 5, 0);
    printf("Ret: %d\n", ret);
    return 0;
}