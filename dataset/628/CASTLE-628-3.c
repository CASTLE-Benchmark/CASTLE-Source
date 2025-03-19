/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-628-3.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-628-3.c -o CASTLE-628-3
vulnerable: true
description: power_mod function call has semantically incorrect parameters.
cwe: 628
============================================================================
*/

#include <stdio.h>
#include <math.h>

long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

int main() {
    // Settings
    long long prime = 37199; // modulus
    long long base = 5;   // generator

    // Private keys
    long long alice_private = 19945234904;  // Alice (secret)
    long long bob_private = 2530476561349134275;   // Bob (secret)

    long long alice_public = power_mod(base, alice_private, prime);
    printf("Alice's public key: %lld\n", alice_public);

    long long bob_public = power_mod(base, bob_private, prime);
    printf("Bob's public key: %lld\n", bob_public);

    long long alice_shared_secret = power_mod(bob_public, alice_private, prime);
    printf("Alice's shared secret: %lld\n", alice_shared_secret);

    long long bob_shared_secret = power_mod(bob_private, alice_public, prime); // {!LINE}
    printf("Bob's shared secret: %lld\n", bob_shared_secret);

    if (alice_shared_secret == bob_shared_secret) {
        printf("Key exchange successful! Shared secret: %lld\n", alice_shared_secret);
    } else {
        printf("Key exchange failed.\n");
    }

    return 0;
}
