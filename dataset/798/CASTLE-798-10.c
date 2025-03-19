/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-798-10.c
author: Richard Dubniczky
version: 1.1
compile: gcc CASTLE-798-10.c -o CASTLE-798-10
vulnerable: false
description: The Google Maps API token is not a secret. It is publicly accessible embedded into the web page.
cwe: 798
============================================================================
*/

#include <stdio.h>

int main() {
    const char *googleMapsApiKey = "AIzaSyA-kDsNrByA6bsKrsgKwGwQWeDyVRUgbmBTmvTRJnQafF";
    printf("<script async defer src=\"https://maps.googleapis.com/maps/api/js?key=%s&callback=initMap\"></script>", googleMapsApiKey);
    return 0;
}