/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-125-10.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-125-10.c -o CASTLE-125-10
vulnerable: false
description: Number of countries is returned correctly
cwe: 125
============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char code[4];
    int population; // Million
} Country;

Country* getCountries() {
    Country* countries = (Country*)malloc(sizeof(Country) * 9);
    if (countries == NULL) {
        printf("Failed to allocate memory.\n");
        exit(1); // Indicate error and exit
    }

    strncpy(countries[0].code, "HUN", 4);
    countries[0].population = 10;
    strncpy(countries[1].code, "GER", 4);
    countries[1].population = 84;
    strncpy(countries[2].code, "FRA", 4);
    countries[2].population = 66;
    strncpy(countries[3].code, "ITA", 4);
    countries[3].population = 59;
    strncpy(countries[4].code, "SPA", 4);
    countries[4].population = 48;
    strncpy(countries[5].code, "POL", 4);
    countries[5].population = 38;
    strncpy(countries[6].code, "SWE", 4);
    countries[6].population = 10;
    strncpy(countries[7].code, "GRE", 4);
    countries[7].population = 10;
    strncpy(countries[8].code, "AUT", 4);
    countries[8].population = 9;

    return countries;
}

int getNumberOfCountries(Country* c) {
    if (c != NULL) {
        return 9;
    }
    return 0;
}

int main() {
    Country* countries = getCountries();
    int numOfCountries = getNumberOfCountries(countries);
    for (int i = 0; i < numOfCountries; ++i) {
        printf("%s: %d 000 000\n", countries[i].code, countries[i].population);
    }
    free(countries);

    return 0;
}