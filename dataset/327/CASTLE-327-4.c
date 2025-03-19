/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-327-4.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-327-4.c -o CASTLE-327-4 -lssl -lcrypto
vulnerable: true
description: RSA is used without padding.
cwe: 327
============================================================================
*/

// openssl genrsa -out rsa.pem 2048
// openssl rsa -in rsa.pem -outform PEM -pubout -out rsa.pem.pub

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/decoder.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void cleanUp(BIO* bio, EVP_PKEY* pkey, OSSL_DECODER_CTX* dctx, EVP_PKEY_CTX* ctx) {
    BIO_free_all(bio);
    OSSL_DECODER_CTX_free(dctx);
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
}

void bufferToHexString(unsigned char* buffer, char* string, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        sprintf(string + (i * 2), "%02x", buffer[i]);
    }
    string[len * 2] = '\0';
}

char* rsa_encrypt(const char* plaintext, const char* key) {
    BIO* bio = NULL;
    EVP_PKEY* pkey = NULL;
    OSSL_DECODER_CTX* dctx = NULL;
    EVP_PKEY_CTX* ctx = NULL;

    const size_t inLength = strlen(plaintext);
    const size_t keyLength = strlen(key);
    if (inLength == 0 || keyLength == 0) {
        return NULL;
    }

    bio = BIO_new_mem_buf(key, keyLength);
    if (bio == NULL) {
        cleanUp(bio, pkey, dctx, ctx);
        return NULL;
    }

    dctx = OSSL_DECODER_CTX_new_for_pkey(&pkey, "PEM", NULL, "RSA", EVP_PKEY_PUBLIC_KEY, NULL, NULL);
    if (dctx == NULL) {
        cleanUp(bio, pkey, dctx, ctx);
        return NULL;
    }

    if (!OSSL_DECODER_from_bio(dctx, bio)) {
        cleanUp(bio, pkey, dctx, ctx);
        return NULL;
    }

    ctx = EVP_PKEY_CTX_new(pkey, NULL);
    if (ctx == NULL) {
        cleanUp(bio, pkey, dctx, ctx);
        return NULL;
    }

    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        cleanUp(bio, pkey, dctx, ctx);
        return NULL;
    }

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_NO_PADDING) <= 0) { // {!LINE}
        cleanUp(bio, pkey, dctx, ctx);
        return NULL;
    }

    size_t length;
    if (EVP_PKEY_encrypt(ctx, NULL, &length, (unsigned char*)plaintext, inLength) <= 0) {
        cleanUp(bio, pkey, dctx, ctx);
        return NULL; 
    }

    unsigned char* ciphertext = (unsigned char*)malloc(length);
    if (ciphertext == NULL) {
        cleanUp(bio, pkey, dctx, ctx);
        return NULL;
    }

    if (EVP_PKEY_encrypt(ctx, ciphertext, &length, (unsigned char*)plaintext, inLength) <= 0) {
        cleanUp(bio, pkey, dctx, ctx);
        free(ciphertext);
        return NULL; 
    }

    char* hexCiphertext = (char*)malloc(length * 2 + 1);
    if (hexCiphertext == NULL) {
        cleanUp(bio, pkey, dctx, ctx);
        free(ciphertext);
        return NULL;
    }

    bufferToHexString(ciphertext, hexCiphertext, length);
    free(ciphertext);
    cleanUp(bio, pkey, dctx, ctx);
    return hexCiphertext;
}

char* readKey(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    const size_t length = ftell(file);
    if (length == 0xFFFFFFFFFFFFFFFF) {
        fclose(file);
        return NULL;
    }

    char* buffer = (char*)malloc(length + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    rewind(file);
    const size_t read = fread(buffer, 1, length, file);
    if (read != length) {
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

int main() {
    char* key = readKey("rsa.pem.pub");
    if (key == NULL) {
        return 1;
    }

    char sensitiveText[1024];
    printf("Sensitive text: ");
    fgets(sensitiveText, sizeof(sensitiveText), stdin);
    sensitiveText[strcspn(sensitiveText, "\n")] = '\0';

    char* ciphertext = rsa_encrypt(sensitiveText, key);
    if (ciphertext == NULL) {
        free(key);
        return 1;
    }

    printf("Encrypted: %s\n", ciphertext);
    memset(sensitiveText, 0, sizeof(sensitiveText));
    memset(key, 0, strlen(key));
    free(key);
    free(ciphertext);

    return 0;
}
