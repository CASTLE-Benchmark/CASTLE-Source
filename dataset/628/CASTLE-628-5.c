/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-628-5.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-628-5.c -o CASTLE-628-5
vulnerable: true
description: url_decode function call has semantically incorrect parameters.
cwe: 628
============================================================================
*/

#include <stdio.h>
#include <ctype.h>

int hex_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else {
        return -1;
    }
}

void url_decode(const char *src, char *dest) {
    while (*src) {
        if (*src == '%') {
            if (isxdigit(*(src + 1)) && isxdigit(*(src + 2))) {
                // Decode the next two characters as a hex value
                int high = hex_to_int(*(src + 1));
                int low = hex_to_int(*(src + 2));
                *dest = (char)(high * 16 + low); // {!LINE}
                src += 3;
            } else {
                // Invalid encoding, copy the % as-is
                *dest = *src; // {!LINE}
                src++;
            }
        } else if (*src == '+') {
            // Replace '+' with space
            *dest = ' '; // {!LINE}
            src++;
        } else {
            // Copy other characters as-is
            *dest = *src; // {!LINE}
            src++;
        }
        dest++;
    }
    *dest = '\0'; // {!LINE}
}

int main() {
    char* url = "http://localhost:8080/?search=%3C%3Cscript%3Escript%20%3Ealert(1)%3C/script%20%3C/script%3E%3E";
    char decodedUrl[1024] = {0};

    url_decode(decodedUrl, url); // {!LINE}

    printf("Decoded URL: %s\n", decodedUrl);
    return 0;
}