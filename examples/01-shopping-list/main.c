// 01-shopping-list/main.c
//
// This example teaches you how to use "ParsecList", which is a dynamic array
// that contains char* as items. At the end, the list is deallocated.

#include <stdio.h>

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int main(int argc, char *argv[]) {
    // Initialize parsec with a program name and description
    parsec_init("shopping-list", "Prints your shopping list.");

    // Setup the flags
    ParsecList list;
    parsec_list(
        &list, "-i", "--items", (ParsecList){0},
        "The items in your shopping list."
    );

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    if (!list.len) {
        parsec_help();
        exit(0);
    }

    printf("Your shopping list is %zu items long:\n", list.len);
    for (size_t i = 0; i < list.len; i++) printf("- %s\n", list.items[i]);

    // Deallocate the list
    parsec_da_free(list);

    return 0;
}
