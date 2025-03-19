/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-843-1.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-843-1.c -o CASTLE-843-1
vulnerable: true
description: Explicit casting of incompatible types causes type confusion.
cwe: 843
============================================================================
*/

#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[20];
} Person;

typedef struct {
    int code;
    char description[50];
} Product;

void printInfo(void *data, int type) {
    Person *person = (Person *)data; // {!LINE}
    printf("Person ID: %d\n", person->id);
    printf("Person Name: %s\n", person->name);
}

int main() {
    Person p = {1, "Alice"};
    Product prod = {101, "Gadget"};

    printInfo(&p, 1); // Passing Person data as Product
    printInfo(&prod, 0); // {!LINE} // Passing Product data as Person

    return 0;
}