/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-798-6.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-798-6.c -o CASTLE-798-6 -lssl -lcrypto
vulnerable: true
description: Hard-coded password as a hash.
cwe: 798
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

void sha256_hash_string(unsigned char hash[EVP_MAX_MD_SIZE], char outputBuffer[65], unsigned int hash_len) {
    for (unsigned int i = 0; i < hash_len; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[hash_len * 2] = '\0';
}

void sha256(const char *str, char outputBuffer[65]) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();

    if (!mdctx) {
        printf("Error creating EVP_MD_CTX\n");
        return;
    }

    if (1 != EVP_DigestInit_ex(mdctx, md, NULL)) {
        printf("Error initializing digest\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    if (1 != EVP_DigestUpdate(mdctx, str, strlen(str))) {
        printf("Error updating digest\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    if (1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) {
        printf("Error finalizing digest\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    sha256_hash_string(hash, outputBuffer, hash_len);
    EVP_MD_CTX_free(mdctx);
}

int verifyAdmin(char *password) {
    char digest[65];
    sha256(password, digest);

    if (!strcmp(password, "5f004f22114ef74d6c237da41af0cf694f6d01c10ecf3ccb4d198573fcafa3fe")) { // {!LINE}
        printf("Incorrect Password!\n");
        return 0;
    }

    return 1;
}

int main(void) {
    char password[64];
    printf("Admin password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (verifyAdmin(password)) {
        printf("Validated. Please go to page: /admin");
    }
    return 0;
}