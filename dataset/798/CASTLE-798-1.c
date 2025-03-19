/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-798-1.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-798-1.c -o CASTLE-798-1 -lssl -lcrypto
vulnerable: true
description: The AES key is hard-coded.
cwe: 798
============================================================================
*/

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void handleErrors() {
    fprintf(stderr, "Error occurred\n");
    exit(1);
}

void aes256_encrypt(const char *plaintext, const unsigned char *key, const unsigned char *iv, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    // Create and initialize the context
    if (!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialize the encryption operation with AES-256-CBC
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) handleErrors();

    // Provide the message to be encrypted, and obtain the encrypted output
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char *)plaintext, strlen(plaintext))) handleErrors();

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
}

int main() {
    char message[1024];
    printf("Message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    // 32 bytes
    unsigned char key[32] = { // {!LINE}
        0x60, 0xb7, 0x25, 0xf1, 0x0c, 0x9c, 0x85, 0xc7,
        0x0d, 0x97, 0x88, 0x0d, 0xfe, 0x81, 0x91, 0xb3,
        0xa1, 0xd1, 0x63, 0x87, 0x75, 0xa4, 0xb7, 0x9f,
        0xdd, 0xf5, 0xef, 0x5e, 0xf8, 0x76, 0x1f, 0xb2
    }; // {!LINE}
    // 16 bytes
    unsigned char iv[16] = {
        0x8e, 0xf3, 0xd9, 0xef, 0x4f, 0x4a, 0x5b, 0x6c,
        0x2e, 0x4b, 0x0a, 0x8f, 0x9d, 0xbf, 0x1a, 0x12
    };
    unsigned char ciphertext[2048];

    aes256_encrypt(message, key, iv, ciphertext);

    // Print the encrypted message
    for (int i = 0; i < sizeof(ciphertext); i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    return 0;
}