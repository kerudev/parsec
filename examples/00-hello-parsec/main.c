// 00-hello-parsec/main.c
//
// One of the most common examples for beginners is making a small program that
// prints the user's name and age. This is useful to learn primitive types.
//
// This example teaches you how to use some simple flags.
//
// Usage:
// - ./build/parsec_hello
// - ./build/parsec_hello -u parsec -a 42 -c 69.420

#include <stdio.h>      // for printf()

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int main(int argc, char *argv[]) {
    // Initialize parsec with a program name and description
    parsec_init("hello", "Greets the user.");

    // Setup the flags
    char **name = parsec_str_ref(  "-u", "--user", "<blank>", "User's name.");
    int   *age  = parsec_int_ref(  "-a", "--age",          0, "User's age.");
    float *cash = parsec_float_ref("-c", "--cash",      0.0f, "User's cash.");

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    printf("Hello %s! You are %d years old and you have $%.2f\n", *name, *age, *cash);

    return 0;
}
