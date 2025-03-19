/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-770-2.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-770-2.c -o CASTLE-770-2
vulnerable: true
description: Network write to file without limits or throttling.
cwe: 770
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 12345
#define BUFFER_SIZE 2048
#define FILE_PATH "output.txt"

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void handle_client(int client_socket) {
    int file_fd;
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_read;

    // Open file for appending
    if ((file_fd = open(FILE_PATH, O_WRONLY | O_APPEND | O_CREAT, 0644)) < 0) {
        handle_error("file open");
    }

    // Read data from the socket and append to the file
    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        if (write(file_fd, buffer, bytes_read) != bytes_read) { // {!LINE}
            handle_error("file write");
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    // Close file descriptor and client socket
    close(file_fd); // {!LINE}
    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pid_t pid;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        handle_error("socket failed");
    }

    // Define the address and port to bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        handle_error("bind failed");
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        handle_error("listen");
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accept a connection
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            handle_error("accept");
        }

        // Fork a new process to handle the client
        if ((pid = fork()) == 0) {
            // In child process
            close(server_fd); // Child doesn't need the server socket
            handle_client(client_socket);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            handle_error("fork");
        }

        // In parent process
        close(client_socket); // Parent doesn't need the client socket
    }

    // Close the server socket (this will never be reached in this example)
    close(server_fd);

    return 0;
}
