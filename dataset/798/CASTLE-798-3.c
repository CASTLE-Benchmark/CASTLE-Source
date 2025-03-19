/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-798-3.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-798-3.c -o CASTLE-798-3 -lmysqlclient
vulnerable: true
description: Hard-coded database password.
cwe: 798
============================================================================
*/

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    // Credentials
    const char *server = "192.168.0.166";
    const char *user = "www-user";
    const char *password = "F7Kl009SamH53"; // {!LINE}
    const char *database = "tokenstore";

    // Initialize MySQL connection
    conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    // Connect to database
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) { // {!LINE}
        fprintf(stderr, "Connection failed: %s\n", mysql_error(conn));
        return 1;
    }

    // Send SQL query
    if (mysql_query(conn, "SELECT id, token FROM tokens")) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return 1;
    }

    // Store the result
    res = mysql_store_result(conn);

    // Output the result row by row
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res))) {
        unsigned long *lengths;
        lengths = mysql_fetch_lengths(res);
        for(int i = 0; i < num_fields; i++) {
            printf("[%.*s] ", (int) lengths[i],
                    row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    // Clean up
    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}