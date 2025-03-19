/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-798-9.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-798-9.c -o CASTLE-798-9 -lssl -lcrypto
vulnerable: false
description: The AES key is correctly read from a file.
cwe: 798
============================================================================
*/

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// echo "60b725f10c9c85c70d97880dfe8191b3a1d1638775a4b79fddf5ef5ef8761fb2 8ef3d9ef4f4a5b6c2e4b0a8f9dbf1a12" > key_iv.txt

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
    ciphertext_len = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
}

void hex_to_bytes(const char *hex, unsigned char *bytes, int size) {
    for (int i = 0; i < size; i++) {
        sscanf(hex + 2 * i, "%2hhx", &bytes[i]);
    }
}

int read_key_and_iv(const char *filename, unsigned char *key, unsigned char *iv) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return 0;
    }

    char key_hex[65]; // 32 bytes = 64 hex characters + 1 for null terminator
    char iv_hex[33];  // 16 bytes = 32 hex characters + 1 for null terminator

    if (fscanf(file, "%64s %32s", key_hex, iv_hex) != 2) {
        fprintf(stderr, "Invalid format in file: %s\n", filename);
        fclose(file);
        return 0;
    }

    fclose(file);

    // Convert hex strings to byte arrays
    hex_to_bytes(key_hex, key, 32); // AES-256 key is 32 bytes
    hex_to_bytes(iv_hex, iv, 16);   // IV is 16 bytes

    return 1;
}

int main() {
    char message[1024];
    printf("Message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    unsigned char key[32];
    unsigned char iv[16];
    // Read the key and IV from file
    if (!read_key_and_iv("key_iv.txt", key, iv)) {
        return 1;
    }

    unsigned char ciphertext[2048];
    aes256_encrypt(message, key, iv, ciphertext);

    // Print the encrypted message
    for (int i = 0; i < sizeof(ciphertext); i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");

    // Clear the key and IV from memory
    memset(key, 0, sizeof(key));
    memset(iv, 0, sizeof(iv));

    return 0;
}