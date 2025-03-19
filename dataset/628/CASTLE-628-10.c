/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-628-10.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-628-10.c -o CASTLE-628-10 -lssl -lcrypto
vulnerable: false
description: sha256 function call has semantically correct parameters.
cwe: 628
============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <string_to_hash>\n", argv[0]);
        return 1;
    }

    char digest[65];
    sha256(argv[1], digest);

    printf("%s\n", digest);

    return 0;
}