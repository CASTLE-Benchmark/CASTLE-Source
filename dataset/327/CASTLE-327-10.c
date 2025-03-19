/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-327-10.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-327-10.c -o CASTLE-327-10 -lssl -lcrypto
vulnerable: false
description: HMAC used with SHA256.
cwe: 327
============================================================================
*/

// echo "d25b859e5277340274873697196ba955" > key.txt

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void bufferToHexString(unsigned char* buffer, char* string, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        sprintf(string + (i * 2), "%02x", buffer[i]);
    }
    string[len * 2] = '\0';
}

char* createHMAC(const char* data, const unsigned char* key, size_t keyLen) {
    const size_t dataLen = strlen(data);

    unsigned char* hmac = NULL;
    unsigned int hmacLen = 0;
    hmac = HMAC(EVP_sha256(), key, keyLen, (unsigned char*)data, dataLen, hmac, &hmacLen);

    char* hexHmac = (char*)malloc(2 * hmacLen + 1);
    if (hexHmac == NULL) {
        return NULL;
    }

    bufferToHexString(hmac, hexHmac, hmacLen);
    return hexHmac;
}

int read_key(const char *filename, unsigned char key[16]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return 0;
    }

    char key_hex[33];

    if (fscanf(file, "%32s", key_hex) != 1) {
        fprintf(stderr, "Invalid format in file: %s\n", filename);
        fclose(file);
        return 0;
    }

    fclose(file);

    for (int i = 0; i < 16; i++) {
        sscanf(key_hex + 2 * i, "%2hhx", &key[i]);
    }

    return 1;
}

int main() {
    char sensitiveText[1024];
    printf("Sensitive text: ");
    fgets(sensitiveText, sizeof(sensitiveText), stdin);
    sensitiveText[strcspn(sensitiveText, "\n")] = '\0';

    unsigned char key[16];
    if (!read_key("key.txt", key)) {
        return 1;
    }

    char* hmac = createHMAC(sensitiveText, key, 16);
    if (hmac == NULL) {
        return 1;
    }

    printf("HMAC: %s\n", hmac);
    free(hmac);
    memset(sensitiveText, 0, sizeof(sensitiveText));
    memset(key, 0, sizeof(key));

    return 0;
}