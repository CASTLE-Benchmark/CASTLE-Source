/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-770-1.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-770-1.c -o CASTLE-770-1
vulnerable: true
description: Server allocates process resources without limiting concurrent connections.
cwe: 770
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = read(client_sock, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the received data
        printf("Received: %s\n", buffer);
        write(client_sock, buffer, bytes_read); // Echo data
    }

    close(client_sock);
    exit(0); // Exit child process
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pid_t pid;

    // Create socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Could not bind socket to address");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_sock, 5) == -1) {
        perror("Could not open port");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a connection
        if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
            perror("accept");
            continue;
        }

        // Fork a new process to handle the client
        if ((pid = fork()) == -1) { // {!LINE}
            perror("fork");
            close(client_sock);
            continue;
        }

        if (pid == 0) {
            // Child process
            close(server_sock); // Close the listening socket in the child process
            handle_client(client_sock);
        } else {
            // Parent process
            close(client_sock); // Close the connected socket in the parent process
            waitpid(-1, NULL, WNOHANG); // Reap zombie processes
        }
    }

    close(server_sock);
    return 0;
}