/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-78-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-78-1.c -o CASTLE-78-1
vulnerable: true
description: Missing input validation of fgets input allows for OS command injection
cwe: 78
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    char ip[46]; // ipv6 address length = 45 chars + null

    // Ask for the IP address
    printf("Enter the IP address to ping: ");
    fgets(ip, sizeof(ip), stdin);

    // Create the ping command
    char command[150];
    snprintf(command, sizeof(command), "ping %s", ip); // {!LINE}

    // Execute the ping command
    system(command); // {!LINE}

    return 0;
}