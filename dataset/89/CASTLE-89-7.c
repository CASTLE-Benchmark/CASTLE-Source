/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-89-7.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-89-7.c -o CASTLE-89-7 -lsqlite3
vulnerable: false
description: Static SQL query is not vulnerable to injection.
cwe: 89
============================================================================
*/

// create table students (id INT, name VARCHAR(18), class VARCHAR(5))
// insert into students values (1, 'Arnold', "1b")
// insert into students values (2, 'Claire', "3c")

#include <stdio.h>
#include <sqlite3.h>

int main() {
    sqlite3 *db;
    sqlite3_stmt *res;
    int rc;

    rc = sqlite3_open("students.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Compile the SQL query
    rc = sqlite3_prepare_v2(db, "SELECT * FROM students", -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Execute SQL query
    while (sqlite3_step(res) == SQLITE_ROW) {
        int id = sqlite3_column_int(res, 0);
        const unsigned char *name = sqlite3_column_text(res, 1);
        const unsigned char *grade = sqlite3_column_text(res, 2);

        printf("Name: %s, Grade: %s\n", name, grade);
    }

    // Finalize and clean up
    sqlite3_finalize(res);
    sqlite3_close(db);

    return 0;
}