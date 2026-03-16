// hello-parsec/main.c - Simple program that greets the user.
//
// One of the most common examples for beginners is making a small program that
// prints the user's name and age. This is useful to learn primitive types.
//
// This example  

#include <stdio.h>

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int main(int argc, char *argv[]) {
    // Initialize parsec with a program name and description
    parsec_init("hello", "Greets the user.");

    // Setup the flags
    char *name;
    int age;
    float cash;

    parsec_str(&name, "-u", "--user", "User's name.");
    parsec_int(&age, "-a", "--age", "User's age.");
    parsec_float(&cash, "-c", "--cash", "User's cash.");

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    printf("Hello %s! You are %d years old and you have $%.2f\n", name, age, cash);

    return 0;
}
