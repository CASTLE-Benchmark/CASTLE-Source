/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-787-9.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-787-9.c -o CASTLE-787-9 -lssl -lcrypto
vulnerable: false
description: Password length is bounded below buffer length.
cwe: 787
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/rand.h>

#define MIN_LENGTH 8
#define MAX_LENGTH 16

const char LOWERCASE[] = "abcdefghijklmnopqrstuvwxyz";
const char UPPERCASE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char NUMBERS[] = "0123456789";
const char SPECIAL_CHARACTERS[] = "!@#$%^&*()_-+=[]{}|;:,.<>/?";

unsigned int sslRand () {
    unsigned int randNum;
    int result = RAND_bytes((unsigned char *)&randNum, sizeof(randNum));
    if (result != 1) {
        printf("RAND_bytes failed\n");
        exit(1);
    }
    return randNum;
}

void generatePassword(char password[], int length);

int main() {
    int length;
    char password[MAX_LENGTH+1]; // +1 for null terminator

    printf("Welcome to the C Secure Password Generator!\n");
    printf("Password length must be between %d and %d characters.\n", MIN_LENGTH, MAX_LENGTH);

    do {
        printf("Enter password length: ");
        scanf("%9d", &length);

        if (length < MIN_LENGTH || length > MAX_LENGTH) {
            printf("Password length must be between %d and %d characters.\n", MIN_LENGTH, MAX_LENGTH);
        }
    } while (length < MIN_LENGTH || length > MAX_LENGTH);
    generatePassword(password, length);
    printf("Your randomly generated password is: %s\n", password);

    return 0;
}

void generatePassword(char password[], int length) {
    int i, rnd, category;
    int numCategories = 4; // lowercase, uppercase, numbers, special characters
    int categoriesNeeded = 1; // start with one category (can increase if needed)
    int numCharsInCategory[numCategories];
    int minCharsInCategory = 2; // minimum number of characters needed in a category

    // initialize all categories to zero characters
    memset(numCharsInCategory, 0, sizeof(numCharsInCategory));

    // generate password character by character
    for (i=0; i<length; i++) {
        // decide which category the next character will belong to
        if (categoriesNeeded == 1) {
            // only one category needed, choose randomly
            category = sslRand() % numCategories;
        } else {
            // more than one category needed, choose a category that has not met the minimum requirement
            do {
                category = sslRand() % numCategories;
            } while (numCharsInCategory[category] >= minCharsInCategory);
        }

        // generate a random character from the category chosen
        switch (category) {
            case 0: // lowercase
                rnd = sslRand() % strlen(LOWERCASE);
                password[i] = LOWERCASE[rnd];
                numCharsInCategory[category]++;
                break;

            case 1: // uppercase
                rnd = sslRand() % strlen(UPPERCASE);
                password[i] = UPPERCASE[rnd];
                numCharsInCategory[category]++;
                break;

            case 2: // numbers
                rnd = sslRand() % strlen(NUMBERS);
                password[i] = NUMBERS[rnd];
                numCharsInCategory[category]++;
                break;

            case 3: // special characters
                rnd = sslRand() % strlen(SPECIAL_CHARACTERS);
                password[i] = SPECIAL_CHARACTERS[rnd];
                numCharsInCategory[category]++;
                break;
        }

        // check if all categories have been filled to the minimum requirement
        if (i == length-1 && categoriesNeeded < numCategories) {
            categoriesNeeded++;
        }
    }

    // add null terminator
    password[length] = '\0';
}