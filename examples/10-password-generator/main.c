// 10-password-generator/main.c
//
// This example teaches you how to use some simple flags, then uses the values
// for something else than printing.
//
// Usage:
// - ./build/parsec_password
// - ./build/parsec_password -s 8 -a abcdefghijklmnopqrstuvwxyz

#include <stdio.h>      // for printf()
#include <stdlib.h>     // for srand(), rand()
#include <time.h>       // for time(), clock()
#include <string.h>     // for strlen()

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int get_random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void generate_password(char *allowed, char *password, int size) {
    int allowed_size = strlen(allowed);

    // The seed for random numbers is based on the GNU time and your CPU time
    srand(time(NULL) ^ clock());

    for (int i = 0; i < size; i++) {
        password[i] = allowed[get_random(0, allowed_size - 1)];
    }

    password[size] = '\0';
}

int main(int argc, char **argv) {
    // Initialize parsec with a program name and description
    parsec_init("password", "Generates a random password.");

    // Setup the flags
    size_t size;
    char *allowed;

    parsec_size(
        &size, "-s", "--size", 8,
        "Total characters to generate."
    );
    parsec_str(
        &allowed, "-a", "--allowed", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_",
        "Allowed characters to generate the password."
    );

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    if (!size || !allowed) {
        parsec_help();
        exit(1);
    }

    char password[size + 1];
    generate_password(allowed, password, size);

    printf("Generating a %zu character long password (allowed: %s)\n", size, allowed);
    printf("Your password is: %s\n", password);

    return 0;
}
