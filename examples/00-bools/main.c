// 00-bools/main.c
//
// This example teaches you how to use the bool flags, and also how to simulate
// toggle variants.

#include <stdio.h>

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int main(int argc, char *argv[]) {
    // Initialize parsec with a program name and description
    parsec_init("bools", "Greets the user.");

    // Setup "regular" bool flags
    bool verbose;
    bool dry_run;

    parsec_bool(&verbose, "-v", "--verbose", false, "If provided, verbose printing is ON");
    parsec_bool(&dry_run, "", "--dry-run", true, "If provided, the run will not save any data.");

    // Setup "toggle" bool flags
    bool cov;

    parsec_bool(&cov, "", "--cov", false, "If provided, coverage will be collected");
    parsec_bool(&cov, "", "--no-cov", true, "Opposite of --cov");

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    printf("verbose: %s\n", verbose ? "true" : "false");
    printf("dry-run: %s\n", dry_run ? "true" : "false");
    printf("cov    : %s\n", cov     ? "true" : "false");

    return 0;
}
