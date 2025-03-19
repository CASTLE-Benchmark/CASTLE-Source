/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-522-1.c
author: Richard Dubncizky
version: 1.1
compile: gcc CASTLE-522-1.c -o CASTLE-522-1 -lsqlite3
vulnerable: true
description: Password is stored as plain text in the database.
cwe: 522
============================================================================
*/

// create table users (id INT, username VARCHAR(32), password VARCHAR(32), token VARCHAR(32));
// insert into users values (1, "tom", "password", "1h6df8ddfn");

#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <ctype.h>

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
    snprintf(sql, sizeof(sql), "SELECT * FROM users WHERE username = '%s' and password = '%s'", username, password); // {!LINE}

    // Compile the SQL query
    sqlite3_stmt *res;
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0); // {!LINE}
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Execute SQL query
    int row_count = 0;
    while (sqlite3_step(res) == SQLITE_ROW) {
        const unsigned char *token = sqlite3_column_text(res, 3);
        printf("Login successful. Your token: %s\n", token);
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