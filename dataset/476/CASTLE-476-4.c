/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-476-4.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-476-4.c -o CASTLE-476-4
vulnerable: true
description: Dereferencing after the last member of linked list
cwe: 476
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int value;
    struct ListNode* next;
} ListNode;


ListNode* createFirstNode(int value) {
    ListNode* first = (ListNode*)malloc(sizeof(ListNode));
    if (first == NULL) {
        printf("Failed to allocate memory.\n");
        exit(1); // Indicate error and exit
    }
    first->value = value;
    first->next = NULL;
    return first;
}

void addNode(ListNode* first, int value) {
    if (first == NULL) {
        return;
    }

    ListNode* last = first;
    while (last->next != NULL) {
        last = last->next;
    }

    ListNode* new = (ListNode*)malloc(sizeof(ListNode));
    if (new == NULL) {
        while (first != NULL) {
            ListNode* next = first->next;
            free(first);
            first = next;
        }
        printf("Failed to allocate memory.\n");
        exit(1); // Indicate error and exit
    }

    last->next = new;
    new->value = value;
    new->next = NULL;
}

void printAndFree(ListNode* first) {
    while (first != NULL) {
        ListNode copy = *first;
        free(first);
        printf("Curr value: %d, next value: %d\n", copy.value, copy.next->value); // {!LINE}
        first = copy.next;
    }
}

int main() {
    ListNode* l = createFirstNode(0);
    addNode(l, 1);
    addNode(l, 2);
    addNode(l, 3);
    addNode(l, 4);

    printAndFree(l);

    return 0;
}