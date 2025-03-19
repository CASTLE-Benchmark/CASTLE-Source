/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-89-9.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-89-9.c -o CASTLE-89-9 -lmysqlclient
vulnerable: false
description: Sufficient ingteger valudation of parameter ID does not allow for SQL injection
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

int main(int argc, char *argv[])
{
    MYSQL *conn = 0;
    MYSQL_RES *res = 0;
    MYSQL_ROW row;

    size_t arglen, esclen, fmtlen;

    char *server = "localhost";
    char *user = "root";
    char *database = "bank";
    char *password;
    char *query;
    char *fmtString = "SELECT * FROM users WHERE firstname LIKE '%s'";
    char *param_escaped;
    unsigned int i;

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

    /* Connect to database */
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

    for (i = 1; i < argc; ++i)
    {
        arglen = strlen(argv[i]);
        if (arglen > (SIZE_MAX / sizeof *param_escaped - 1) / 2)
        {
            mysql_close(conn);
            return -1;
        }
        param_escaped = malloc((2 * arglen + 1) * sizeof *param_escaped);
        if (param_escaped == NULL)
        {
            mysql_close(conn);
            return -1;
        }

        esclen = mysql_real_escape_string(conn, param_escaped, argv[i], arglen);

        fmtlen = strlen(fmtString);
        if (esclen > SIZE_MAX / sizeof *query - fmtlen)
        {
            free(param_escaped);
            mysql_close(conn);
            return -1;
        }
        query = malloc((esclen + fmtlen + 1) * sizeof *query);
        if (query == NULL)
        {
            free(param_escaped);
            mysql_close(conn);
            return -1;
        }

        sprintf(query, fmtString, param_escaped);

        if (mysql_query(conn, query))
        {
            free(param_escaped);
            free(query);
            mysql_close(conn);
            return -1;
        }

        res = mysql_use_result(conn);

        while ((row = mysql_fetch_row(res)) != NULL)
            printf("%s %s\n", row[1], row[2]);

        free(param_escaped);
        free(query);
        mysql_free_result(res);
    }

    mysql_close(conn);
    return 0;
}