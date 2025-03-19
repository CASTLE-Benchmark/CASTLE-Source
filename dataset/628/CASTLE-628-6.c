/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-628-6.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-628-6.c -o CASTLE-628-6 -lcurl
vulnerable: true
description: httpGet function call has semantically incorrect parameters.
cwe: 628
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Callback function to handle the response data
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    char *response = (char *)malloc(total_size + 1);
    if (response == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }
    memcpy(response, ptr, total_size);
    response[total_size] = '\0';
    void (*onSuccess)(const char *) = userdata;
    onSuccess(response);
    free(response);
    return total_size;
}

void httpGet(const char *url, void (*onSuccess)(const char *), void (*onError)(const char *)) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, onSuccess);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            onError(curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    } else {
        onError("Failed to initialize CURL.");
    }
}

void onSuccess(const char *response) {
    printf("Success: %s\n", response);
}

void onError(const char *error) {
    fprintf(stderr, "Error: %s\n", error);
}

int main() {
    const char *url = "http://example.com";
    httpGet(url, onError, onSuccess); // {!LINE}
    return 0;
}
