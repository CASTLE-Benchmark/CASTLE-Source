/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-522-6.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-522-6.c -o CASTLE-522-6
vulnerable: true
description: Plain text password is stored in /etc/passwd following convention in legacy UNIX systems.
cwe: 522
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWD_FILE "/etc/passwd"


void add_user(const char *username, const char *password) {
    FILE *passwd_file = fopen(PASSWD_FILE, "a");

    if (passwd_file == NULL) {
        perror("Error opening /etc/passwd");
        exit(1);
    }

    // Format: username:x:uid:gid:comment:home_dir:shell
    fprintf(passwd_file, "%s:%s:1001:1001::/home/%s:/bin/bash\n", username, password, username); // {!LINE}

    fclose(passwd_file);
    printf("User %s added to /etc/passwd with password: %s (INSECURE)\n", username, password);
}

int main() {
    char username[50];
    char password[50];

    printf("Enter new username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;  // Remove newline character

    printf("Enter password for user: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;  // Remove newline character

    // Add user to /etc/passwd
    add_user(username, password);

    return 0;
}