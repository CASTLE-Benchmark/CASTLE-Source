/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-822-10.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-822-10.c -o CASTLE-822-10
vulnerable: false
description: Obtaining value from socket instead of dereferncing avoids segmentation fault (echo "0" | nc localhost 8080)
cwe: 822
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <assert.h>

#define PORT 8080
#define BUFFER_SIZE 64

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d\n", PORT);

    // Accept incoming connection and handle request
    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0) {
        int bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);
        if (bytes_read == -1) {
            close(new_socket);
            continue;
        }
        
        buffer[bytes_read] = '\0';
        printf("Received request: \"%s\"\n", buffer);

        // Process input
        char *endptr;
        unsigned long message = strtoul(buffer, &endptr, 10);
        unsigned long result = message % 2 == 0;
        snprintf(response, sizeof(response), result ? "true\n" : "false\n");
        printf("Sending response: \"%s\"\n", response);

        // Send response
        write(new_socket, response, strlen(response));
        close(new_socket);
    }

    if (new_socket < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return 0;
}