/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-6.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-6.c -o CASTLE-362-6
vulnerable: true
description: Race condition as multiple threads write to the same file
cwe: 362
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define SEQUENCER_LIMIT 1000

volatile sig_atomic_t signal_flag = 0;

void* signal_sequencer(void* arg) {
    int sequence = 0;
    while (sequence < SEQUENCER_LIMIT) {
        if (signal_flag) { // {!LINE}
            printf("%d!\n", sequence);
            sequence++; // {!LINE}
            signal_flag = 0;
        }
        usleep(1000);
    }
    return NULL;
}

void signal_handler(int sig) {
    signal_flag = 1; // {!LINE}
}

int main() {
    pthread_t thread1, thread2;

    // Register signal handler
    signal(SIGUSR1, signal_handler);

    pthread_create(&thread1, NULL, signal_sequencer, NULL);
    pthread_create(&thread2, NULL, signal_sequencer, NULL);

    for (int i = 0; i < 5; i++) {
        printf("Sending SIGUSR1...\n");
        raise(SIGUSR1);
        usleep(500000);
    }

    pthread_cancel(thread1);
    pthread_cancel(thread2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
