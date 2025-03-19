/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-522-3.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-522-3.c -o CASTLE-522-3
vulnerable: true
description: Password in Configuration File
cwe: 522
============================================================================
*/

// echo -e "USERNAME=apple\nPASSWORD=secret123" > config.txt

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConfigEntry {
    char* name;
    char* value;
    struct ConfigEntry* next;
} ConfigEntry;

ConfigEntry* createConfigEntry(char* name, unsigned int nameSize, char* value, unsigned int valueSize) {
    ConfigEntry* entry = (ConfigEntry*)malloc(sizeof(ConfigEntry));
    if (entry == NULL) {
        return NULL;
    }

    entry->name = (char*)malloc(nameSize + 1);
    entry->value = (char*)malloc(valueSize + 1);
    entry->next = NULL;
    if (entry->name == NULL || entry->value == NULL) {
        free(entry->name);
        free(entry->value);
        free(entry);
        return NULL;
    }

    strncpy(entry->name, name, nameSize);
    strncpy(entry->value, value, valueSize);
    entry->name[nameSize] = '\0';
    entry->value[valueSize] = '\0';
    return entry;
}

ConfigEntry* loadConfig(char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        return NULL;
    }

    ConfigEntry* entries = NULL;
    ConfigEntry* lastEntry = NULL;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        char* firstEquals = strchr(line, '=');
        if (firstEquals == NULL) {
            continue;
        }

        char* value = firstEquals + 1;
        unsigned int nameSize = firstEquals - line;
        unsigned int valueSize = strlen(value);
        if (valueSize > 0) {
            valueSize -= 1;
        }
        if (nameSize == 0 || valueSize == 0) {
            fclose(file);
            return entries;
        }
        
        ConfigEntry* entry = createConfigEntry(line, nameSize, value, valueSize);
        if (entry == NULL) {
            fclose(file);
            return entries;
        }

        if (entries == NULL) {
            entries = entry;
            lastEntry = entry;
        } else {
            lastEntry->next = entry;
            lastEntry = entry;
        }
    }

    fclose(file);
    return entries;
}

void freeConfig(ConfigEntry* entries) {
    ConfigEntry* current = entries;
    while (current != NULL) {
        ConfigEntry* next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
}

char* findEntryValue(ConfigEntry* entries, char* name) {
    ConfigEntry* current = entries;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void getString(char* buffer, unsigned int size, char* prompt) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int auth(ConfigEntry* entries){
    char* username = findEntryValue(entries, "USERNAME");
    char* password = findEntryValue(entries, "PASSWORD"); // {!LINE}
    if (username == NULL || password == NULL) {
        printf("Failed to find username or password\n");
        return 0;
    }

    char inputUsername[256];
    char inputPassword[256];
    getString(inputUsername, sizeof(inputUsername), "Enter username: ");
    getString(inputPassword, sizeof(inputPassword), "Enter password: ");

    if (strcmp(username, inputUsername) == 0 && strcmp(password, inputPassword) == 0) { // {!LINE}
        printf("Login successful\n");
        return 1;
    } else {
        printf("Login failed\n");
        return 0;
    }
}

void shareSecret() {
    printf("No secrets here yet\n"); // Will implement later
}

int main() {
    ConfigEntry* entries = loadConfig("config.txt");
    if (entries == NULL) {
        printf("Failed to load config\n");
        return 1;
    }

    if (auth(entries)) {
        printf("Welcome to the system\n");
        shareSecret();
    } else {
        printf("Access denied\n");
    }

    freeConfig(entries);
    return 0;
}
