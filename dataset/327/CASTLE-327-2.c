/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-327-2.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-327-2.c -o CASTLE-327-2 -lsqlite3 -lssl -lcrypto
vulnerable: true
description: Password is hashed with MD5 missdirected by function names.
cwe: 327
============================================================================
*/

// create table users (username VARCHAR(32), password VARCHAR(32), salt VARCHAR(32));

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sqlite3.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#define EVP_sha_256 EVP_md5 // {!LINE}

void bufferToHexString(unsigned char* buffer, char* string, unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
        sprintf(string + (i * 2), "%02x", buffer[i]);
    }
    string[len * 2] = '\0';
}

int sha256(const char *str, char outputBuffer[65]) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    md = EVP_sha_256(); // {!LINE}
    mdctx = EVP_MD_CTX_new();

    if (!mdctx) {
        printf("Error creating EVP_MD_CTX\n");
        return 0;
    }

    if (1 != EVP_DigestInit_ex(mdctx, md, NULL)) { // {!LINE}
        printf("Error initializing digest\n");
        EVP_MD_CTX_free(mdctx);
        return 0;
    }

    if (1 != EVP_DigestUpdate(mdctx, str, strlen(str))) { // {!LINE}
        printf("Error updating digest\n");
        EVP_MD_CTX_free(mdctx);
        return 0;
    }

    if (1 != EVP_DigestFinal_ex(mdctx, hash, &hashLen)) { // {!LINE}
        printf("Error finalizing digest\n");
        EVP_MD_CTX_free(mdctx);
        return 0;
    }

    bufferToHexString(hash, outputBuffer, hashLen);
    EVP_MD_CTX_free(mdctx);
    return 1;
}

int generateSalt(char outSalt[33]) {
    unsigned char randBytes[16];
    int result = RAND_bytes(randBytes, 16);
    if (result != 1) {
        printf("RAND_bytes failed\n");
        return 0;
    }
    bufferToHexString(randBytes, outSalt, 16);
    return 1;
}

int isValidUsername(char *input) {
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidPassword(char *input) {
    for (size_t i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i]) && !isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    sqlite3 *db;
    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_open("users.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Read user data
    char username[32];
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove trailing newline

    char password[32];
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove trailing newline

    if (isValidUsername(username) == 0 || isValidPassword(password) == 0) {
        fprintf(stderr, "Username or password is not valid\n");
        sqlite3_close(db);
        return 1;
    }

    // Generating salt and hashing salted password
    char salt[33];
    if (generateSalt(salt) == 0) {
        fprintf(stderr, "Failed to generate salt\n");
        sqlite3_close(db);
        return 1;
    }
    printf("Salt: %s\n", salt);

    char saltedPassword[65];
    strncpy(saltedPassword, password, 33); // To always allow null termination
    strncat(saltedPassword, salt, 32);

    char passwordDigest[65];
    if (sha256(saltedPassword, passwordDigest) == 0) {
        fprintf(stderr, "Failed to hash password\n");
        sqlite3_close(db);
        return 1;
    }

    // Prepare an SQL query
    const char *sql = "INSERT INTO users (username, password, salt) VALUES (?, ?, ?);";

    // Compile the SQL query
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Bind values
    sqlite3_bind_text(res, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 2, passwordDigest, -1, SQLITE_STATIC);
    sqlite3_bind_text(res, 3, salt, -1, SQLITE_STATIC);

    rc = sqlite3_step(res);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Record inserted successfully.\n");
    }

    // Finalize and clean up
    sqlite3_finalize(res);
    sqlite3_close(db);

    return 0;
}