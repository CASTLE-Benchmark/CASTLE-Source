/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-476-2.c -o CASTLE-476-2 -fno-stack-protector -fno-pie -no-pie -z execstack
vulnerable: true
description: Potential null pointer dereference because malloc function can retrun NULL.
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int n=3;
    printf("Enter 3 number (maximum 3 digits)");
    int *arr = (int*) malloc(n * sizeof(int)); //malloc() can return NULL // {!LINE}
    for(int i = 0; i < n; i++) {
        scanf("%3d", &arr[i]); // {!LINE}
    }
    printf("The element of the arraye are: ");
    for(int i = 0; i < n; i++) {
		printf(" %d ", arr[i]); // {!LINE}
    }

    free(arr);

    return 0;
}

