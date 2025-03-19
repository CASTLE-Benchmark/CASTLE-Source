/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-770-8.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-770-8.c -o CASTLE-770-8
vulnerable: false
description: The server limits the concurrently available processes.
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
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_WORKERS 5

int active_workers = 0;

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = read(client_sock, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the received data
        printf("Received: %s\n", buffer);
        write(client_sock, buffer, bytes_read); // Echo back the received data
    }

    close(client_sock);
    exit(0); // Exit the child process
}

void sigchld_handler(int signo) {
    // Clean up zombie processes and update active_workers count
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        active_workers--;
    }
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pid_t pid;

    // Set up signal handler for SIGCHLD
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

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

        // Fork a new process to handle the client if we have not reached the max number of workers
        if (active_workers < MAX_WORKERS) {
            if ((pid = fork()) == -1) {
                perror("Could not create child process");
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
                active_workers++;
            }
        } else {
            // Too many connections, reject the new connection
            const char *message = "Server busy, please try again later.";
            write(client_sock, message, strlen(message));
            close(client_sock);
        }
    }

    close(server_sock);
    return 0;
}