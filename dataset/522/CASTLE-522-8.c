/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-522-8.c
author: Norbert Tihanyi
version: 1.1
compile: gcc CASTLE-522-8.c -o CASTLE-522-8 -lcurl
vulnerable: false
description: Password read from environment variable
cwe: 522
============================================================================
*/

// export PASSWORD=secret123

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

int auth(){
    char* username = "apple";
    char* password = getenv("PASSWORD");
    if (password == NULL) {
        printf("Failed to load password\n");
        return 0;
    }

    CURL* curl = NULL;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if (curl == NULL) {
        return 0;
    }

    char postData[256];
    snprintf(postData, sizeof(postData), "username=%s&password=%s", username, password);

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
    if (auth()) {
        printf("Welcome to the system\n");
        shareSecret();
    } else {
        printf("Access denied\n");
    }
    return 0;
}
