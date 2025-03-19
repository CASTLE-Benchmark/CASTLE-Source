/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-798-7.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-798-7.c -o CASTLE-798-7 -lcurl -lssl -lcrypto
vulnerable: false
description: AWS credentials are loaded from environment variables.
cwe: 798
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <time.h>

// Constants for S3 request
const char *bucket_name = "gd58sz08-production";
const char *region = "us-east-1";

// Function to generate the current timestamp in AWS format
void get_amz_date(char *amz_date, char *date_stamp) {
    time_t t = time(NULL);
    struct tm *tm_info = gmtime(&t);

    strftime(amz_date, 17, "%Y%m%dT%H%M%SZ", tm_info);  // amz_date format: YYYYMMDD'T'HHMMSS'Z'
    strftime(date_stamp, 9, "%Y%m%d", tm_info);         // date_stamp format: YYYYMMDD
}

// Function to create HMAC SHA256
void hmac_sha256(const char *key, const char *data, unsigned char *hmac_result) {
    unsigned int len = 32;
    HMAC(EVP_sha256(), key, strlen(key), (unsigned char*)data, strlen(data), hmac_result, &len);
}

// Function to calculate the AWS Signature
void calculate_signature(const char *string_to_sign, const char *secret_key, const char *date_stamp, const char *region, char *signature) {
    unsigned char k_date[32], k_region[32], k_service[32], k_signing[32], hmac_result[32];
    char date_key[64];

    snprintf(date_key, sizeof(date_key), "AWS4%s", secret_key);
    hmac_sha256(date_key, date_stamp, k_date);
    hmac_sha256((char*)k_date, region, k_region);
    hmac_sha256((char*)k_region, "s3", k_service);
    hmac_sha256((char*)k_service, "aws4_request", k_signing);
    hmac_sha256((char*)k_signing, string_to_sign, hmac_result);

    for (int i = 0; i < 32; i++) {
        sprintf(&signature[i * 2], "%02x", (unsigned int)hmac_result[i]);
    }

    signature[64] = '\0';
}

// Callback to handle response data from libcurl
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    fwrite(ptr, size, nmemb, stdout);  // Print the response to stdout
    return size * nmemb;
}

int main() {
    const char *access_key = getenv("AWS_ACCESS_KEY_ID");
    const char *secret_key = getenv("AWS_SECRET_ACCESS_KEY");
    if (access_key == NULL || secret_key == NULL) {
        return 1;
    }

    char amz_date[17], date_stamp[9];
    get_amz_date(amz_date, date_stamp);

    // Create the canonical request
    char canonical_request[1024];
    snprintf(canonical_request, sizeof(canonical_request),
             "GET\n"
             "/%s\n"
             "\n"
             "host:%s.s3.%s.amazonaws.com\n"
             "x-amz-date:%s\n"
             "\n"
             "host;x-amz-date\n"
             "UNSIGNED-PAYLOAD",
             bucket_name, bucket_name, region, amz_date);

    // Create the string to sign
    char string_to_sign[1024];
    snprintf(string_to_sign, sizeof(string_to_sign),
             "AWS4-HMAC-SHA256\n"
             "%s\n"
             "%s/%s/s3/aws4_request\n"
             "UNSIGNED-PAYLOAD",
             amz_date, date_stamp, region);

    // Calculate the signature
    char signature[65];
    calculate_signature(string_to_sign, secret_key, date_stamp, region, signature);

    // Create the authorization header
    char authorization_header[1024];
    snprintf(authorization_header, sizeof(authorization_header),
             "AWS4-HMAC-SHA256 Credential=%s/%s/%s/s3/aws4_request, SignedHeaders=host;x-amz-date, Signature=%s",
             access_key, date_stamp, region, signature);

    // Prepare the request URL
    char url[256];
    snprintf(url, sizeof(url), "https://%s.s3.%s.amazonaws.com", bucket_name, region);

    // Initialize CURL
    CURL *curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/xml");
        headers = curl_slist_append(headers, authorization_header);  // Add Authorization header
        headers = curl_slist_append(headers, amz_date);              // Add x-amz-date header

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return 0;
}