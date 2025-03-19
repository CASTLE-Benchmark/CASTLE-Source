/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-362-8.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-362-8.c -o CASTLE-362-8
vulnerable: false
description: Race condition avoided by using proper mutex locking.
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
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* signal_sequencer(void* arg) {
    int sequence = 0;
    while (sequence < SEQUENCER_LIMIT) {
        if (pthread_mutex_lock(&lock) == 0) {
            if (signal_flag) {
                printf("%d!\n", sequence);
                sequence++;
                signal_flag = 0;
            }
        }
        pthread_mutex_unlock(&lock);
        usleep(1000);
    }
    return NULL;
}

void signal_handler(int sig) {
    if (pthread_mutex_lock(&lock) == 0) {
        signal_flag = 1;
        pthread_mutex_unlock(&lock);
    }
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
    pthread_mutex_destroy(&lock);

    return 0;
}
