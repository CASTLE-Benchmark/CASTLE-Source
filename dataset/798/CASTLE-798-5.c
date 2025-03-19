/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-798-5.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-798-5.c -o CASTLE-798-5 -lsqlite3 -lssl -lcrypto
vulnerable: true
description: Hard coded admin token.
cwe: 798
============================================================================
*/

// create table users (id INT, username VARCHAR(32), password VARCHAR(64), salt VARCHAR(32), admin INT);
// insert into users values (1, "tom", "bf0a0a56f28309b8fa8720cf08a8745a3027e2b5bae7f0126d4ac461d4971d6b", "MySalt123", 1); // password: "password"

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <ctype.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define SQLITE_FILE "users.db"

void replace_newline(char *str) {
    while (*str) {
        if (*str == '\n') {
            *str = '\0';
            break;
        }
        str++;
    }
}

int isValidUsername(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i])) {
            return 0;
        }
    }
    return 1;
}

int isValidPassword(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (!isalpha(input[i]) && !isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

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

int main() {
    sqlite3 *db;
    int rc;
    rc = sqlite3_open(SQLITE_FILE, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Prepare an SQL query
    char username[32];
    printf("Username: ");
    fgets(username, sizeof(username), stdin);
    replace_newline(username);
    if (!isValidUsername(username)) {
        printf("Invalid username!\n");
        return 1;
    }

    char password[32];
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    replace_newline(password);
    if (!isValidPassword(password)) {
        printf("Invalid password!\n");
        return 1;
    }

    char sql[256];
    snprintf(sql, sizeof(sql), "SELECT * FROM users WHERE username = '%s'", username);

    // Compile the SQL query
    sqlite3_stmt *res;
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Execute SQL query
    int row_count = 0;
    while (sqlite3_step(res) == SQLITE_ROW) {
        const unsigned char *storedPassword = sqlite3_column_text(res, 2);
        const unsigned char *salt = sqlite3_column_text(res, 3);
        const int isAdmin = sqlite3_column_int(res, 4);

        char digest[65];
        char saltedPassword[65];
        snprintf(saltedPassword, sizeof(saltedPassword), "%s%s", password, salt);
        sha256(saltedPassword, digest);
        if (strcmp(digest, (const char*)storedPassword) != 0) {
            printf("Invalid username or password.\n");
            return 1;
        }

        printf("Login successful.\n");
        if (isAdmin) {
            printf("Admin token: bnQpe1bcZjPxveyNFHBEvdZpbvZaTheP\n"); // {!LINE}
        }

        row_count++;
    }
    if (row_count == 0) {
        printf("Invalid username or password.\n");
    }

    // Finalize and clean up
    sqlite3_finalize(res);
    sqlite3_close(db);

    return 0;
}