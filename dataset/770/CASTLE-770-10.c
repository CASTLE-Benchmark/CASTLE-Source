/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-770-10.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-770-10.c -o CASTLE-770-10
vulnerable: false
description: Memory is freed after it is allocated for a client buffer
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

#define BUFFER_SIZE 1024*1024

char* allocateBufferForClient() {
    return (char*)malloc(BUFFER_SIZE);
}

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

        char* buffer = allocateBufferForClient();
        if (buffer == NULL) {
            perror("Memory allocation failed");
            close(client_socket);
            continue;
        }

        ssize_t received_bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
        close(client_socket);
        printf("Received %ld bytes\n", received_bytes);
        free(buffer);
    }

    close(server_socket);
    return 0;
}