/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-522-4.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-522-4.c -o CASTLE-522-4 -lcurl
vulnerable: true
description: Password in Configuration File (echo -e "UN=apple\nPD=secret123" > config.txt)
cwe: 522
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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

int auth(ConfigEntry* entries){
    char* username = findEntryValue(entries, "UN");
    char* not_password = findEntryValue(entries, "PD"); // {!LINE}
    if (username == NULL || not_password == NULL) {
        printf("Failed to find username or password\n");
        return 0;
    }

    CURL* curl = NULL;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if (curl == NULL) {
        return 0;
    }

    char postData[256];
    snprintf(postData, sizeof(postData), "username=%s&password=%s", username, not_password); // {!LINE}

    curl_easy_setopt(curl, CURLOPT_URL, "https://mypage.com/login");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
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
