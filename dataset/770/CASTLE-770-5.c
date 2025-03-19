/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-770-5.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-770-5.c -o CASTLE-770-5
vulnerable: true
description: Unlimited forking during connection handling may cause memory exhaustion.
cwe: 770
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("A connection has been accepted\n");

        pid_t pid = fork(); // {!LINE}
        if (pid < 0) {
            perror("Fork failed");
            close(client_socket);
            continue;
        }

        if (pid == 0) {
            close(client_socket);
            exit(0);
        } else {
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}