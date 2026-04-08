// 02-strings/main.c
//
// This example teaches you how to use "ParsecString", which is a basic string
// view implementation.
//
// If you are not familiar with string views, they store a char* and its length.
// This allows you to manipulate a string by its length, not by where `\0` is.
// 
// Benefits of this approach:
// - You can modify the data without allocations.
// - The original string stays the same (no data destruction).
//
// Usage:
// - ./build/parsec_strings -s foobarbaz -v foobarbaz

#include <stdio.h>
#include <string.h>

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int main(int argc, char **argv) {
    // Initialize parsec with a program name and description
    parsec_init("strings", "Takes string-like flags and prints their values.");

    // Setup flags
    char *str;
    ParsecString view;

    parsec_str(
        &str, "-s", "--str", "",
        "Flag that will be converted to char *"
    );

    parsec_string(
        &view,  "-v", "--view", (ParsecString){0},
        "Flag that will be converted to ParsecString"
    );

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    if (!str || !view.len) {
        parsec_help();
        exit(1);
    }

    if (strlen(str) < 8) {
        fprintf(stderr, "-s/--str must be at least 8 chars long\n");
        exit(1);
    }

    if (view.len < 8) {
        fprintf(stderr, "-v/--view must be at least 8 chars long\n");
        exit(1);
    }

    printf("Printing the whole string\n");
    printf("- char*       : %s (length = %zu)\n", str, strlen(str));
    printf("- ParsecString: %s (length = %zu)\n", view.str, view.len);

    printf("\n");

    // Shorten the strings by 2 characters on both sides
    char *s = parsec_str_clone(str);
    s += 2;                 // chop left
    s[strlen(s) - 2] = 0;   // chop right

    view.str += 2;          // chop left
    view.len -= 2;
    view.len -= 2;          // chop right

    printf("Printing the whole string (after chopping)\n");
    printf("- char*       : %s   (length = %zu)\n", s, strlen(s));
    printf("- ParsecString: %s (length = %zu)\n", view.str, view.len);

    // Noticed how ParsecString printed 6 characters instead of 4? This is
    // because the string inside it is being printed until '\0'. The next block
    // teaches you how to print both strings by length.

    printf("\n");

    printf("Printing by length\n");
    printf("- char*       : %.*s (length = %zu)\n", (int)strlen(s), s, strlen(s));
    printf("- ParsecString: %.*s (length = %zu)\n", (int)view.len, view.str, view.len);

    // Also note that freeing is easier with views (or unrequired)
    s -= 2;
    s[strlen(s) + 2] = ' ';
    free(s);

    return 0;
}
