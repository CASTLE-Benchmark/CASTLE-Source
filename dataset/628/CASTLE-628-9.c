/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-628-9.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-628-9.c -o CASTLE-628-9
vulnerable: false
description: url_decode function call has semantically correct parameters.
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
                *dest = (char)(high * 16 + low);
                src += 3;
            } else {
                // Invalid encoding, copy the % as-is
                *dest = *src;
                src++;
            }
        } else if (*src == '+') {
            // Replace '+' with space
            *dest = ' ';
            src++;
        } else {
            // Copy other characters as-is
            *dest = *src;
            src++;
        }
        dest++;
    }
    *dest = '\0';
}

int main() {
    char* url = "http://localhost:8080/?search=%3C%3Cscript%3Escript%20%3Ealert(1)%3C/script%20%3C/script%3E%3E";
    char decodedUrl[1024];

    url_decode(url, decodedUrl);

    printf("Decoded URL: %s\n", decodedUrl);
    return 0;
}