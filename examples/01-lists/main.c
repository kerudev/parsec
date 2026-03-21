// 01-lists/main.c
//
// This example teaches you how to use "ParsecList", which is a dynamic array
// that contains char* as items.
//
// You can also convert a string to ParsecList.
//
// Usage:
// - ./build/parsec_lists -s foo,bar,baz,qux -c , -l foo,bar,baz,qux
// - ./build/parsec_lists -s foo,bar,baz,qux -l foo,bar,baz,qux

#include <stdio.h>

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int main(int argc, char *argv[]) {
    // Initialize parsec with a program name and description
    parsec_init("lists", "Takes list-like arguments.");

    // Setup the flags
    char *str;
    char sep;
    ParsecList list;

    parsec_str(
        &str, "-s", "--str", "",
        "The string that will be converted to ParsecList."
    );

    parsec_char(
        &sep, "-c", "--sep", ',',
        "The items in your shopping list."
    );

    parsec_list(
        &list, "-l", "--list", (ParsecList){0},
        "The items of your list."
    );

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    char delimiter[2] = {sep, '\0'};
    ParsecList parsed = parsec_str_to_list(str, delimiter);

    if (parsed.len < 3) {
        printf("-s/--str must be at least 4 items long (comma separated by default)\n");
        exit(1);
    }

    if (list.len < 3) {
        printf("-l/--list must be at least 4 items long (comma separated by default)\n");
        exit(1);
    }

    printf("-s/--str : the list is %zu items long.\n", parsed.len);
    for (size_t i = 0; i < list.len; i++) printf("  - %s\n", parsed.items[i]);

    printf("\n");

    printf("-l/--list: the list is %zu items long.\n", list.len);
    for (size_t i = 0; i < list.len; i++) printf("  - %s\n", list.items[i]);

    // Deallocate the lists
    parsec_list_free(parsed);
    parsec_list_free(list);

    return 0;
}
