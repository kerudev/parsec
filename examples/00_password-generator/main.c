// password-generator/main.c - Simple password generator.
//
// This example uses 

#include <stdio.h>
#include <stdlib.h>  // for srand(), rand()
#include <time.h>    // for time(), clock()

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int get_random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void generate_password(char *password, int size) {
    char allowed[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    int allowed_size = sizeof(allowed) - 1;

    srand(time(NULL) ^ clock());

    for (int i = 0; i < size; i++) {
        password[i] = allowed[get_random(0, allowed_size - 1)];
    }

    password[size] = '\0';
}

int main(int argc, char *argv[]) {
    // Initialize parsec with a program name and description
    parsec_init("rndpwd", "Generates a random password.");

    // Setup the flags
    int size;
    parsec_int(&size, "-s", "--size", "Total characters to generate.");

    // Parse the flags
    parsec_parse(argc, argv);

    char password[size + 1];
    generate_password(password, size);

    printf("Generating a %d character long password\n", size);
    printf("Your password is: %s\n", password);

    return 0;
}
