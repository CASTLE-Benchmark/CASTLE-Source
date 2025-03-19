/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-89-6.c
author: Richard Dubniczky, https://samate.nist.gov/SARD/test-cases/149099/versions/2.0.0
version: 1.1
compile: gcc CASTLE-89-6.c -o CASTLE-89-6 -lmysqlclient
vulnerable: true
description: Insufficient input validation hidden in subfunction allows for SQL injection.
cwe: 89
============================================================================
*/

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>


char *my_memset_s(char *s, int c, size_t n)
{
    volatile char *p = s;

    if (p != NULL)
        while (n--)
            *p++ = c;

    return s;
}

char *getpass_r(const char *prompt)
{
    struct termios oflags, nflags;
    char password[64] = {'\0'};
    char *ret = NULL;

    /* Disabling echo */
    if (tcgetattr(fileno(stdin), &oflags))
        return NULL;

    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSAFLUSH, &nflags))
        return NULL;

    /* Read the password */
    fprintf(stderr, "%s", prompt);
    ret = fgets(password, sizeof(password), stdin);

    /* Restore echo */
    if (tcsetattr(fileno(stdin), TCSAFLUSH, &oflags))
    {
        my_memset_s(password, 0, sizeof(password));
        return NULL;
    }

    if (ret)
    {
        strtok(password, "\n");
        ret = strdup(password);
        my_memset_s(password, 0, sizeof(password));
    }

    return ret;
}

int runSQLCommand(MYSQL *conn, const char *query)
{
    if (mysql_query(conn, query))
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    size_t arglen, fmtlen;

    char *server = "localhost";
    char *user = "root";
    char *database = "bank";
    char *password;
    char *query;
    char *fmtString = "SELECT * FROM users WHERE firstname LIKE '%s'";

    if (argc < 2)
    {
        printf("You should give an entry parameter!\n");
        return -1;
    }

    conn = mysql_init(NULL);
    if (!conn)
        return -1;

    password = getpass_r("DB Password: ");
    if (!password)
    {
        mysql_close(conn);
        return -1;
    }

    if (!mysql_real_connect(conn, server,
                            user, password, database, 0, NULL, 0))
    {
        my_memset_s(password, 0, strlen(password));
        free(password);
        mysql_close(conn);
        return -1;
    }

    my_memset_s(password, 0, strlen(password));
    free(password);


    arglen = strlen(argv[1]);
    fmtlen = strlen(fmtString);
    if (arglen > SIZE_MAX / sizeof *query - fmtlen)
    {
        // the operation would cause an integer overflow
        mysql_close(conn);
        return -1;
    }
    query = malloc((arglen + fmtlen) * sizeof *query);
    if (query == NULL)
    {
        mysql_close(conn);
        return -1;
    }

    sprintf(query, fmtString, argv[1]); // {!LINE}

    if (runSQLCommand(conn, (const char *)query)) // {!LINE}
    {
        free(query);
        mysql_close(conn);
        return -1;
    }

    res = mysql_use_result(conn);

    /* output fields 1 and 2 of each row */
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s %s\n", row[1], row[2]);

    /* Release memory used to store results and close connection */
    free(query);
    mysql_free_result(res);
    mysql_close(conn);
    return 0;
}