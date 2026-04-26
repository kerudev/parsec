// 01-bools/main.c
//
// This example teaches you how to use the bool flags, and also how to simulate
// toggle variants.
//
// Usage:
// - ./build/parsec_bools
// - ./build/parsec_bools -v --dry-run --no-cov

#include <stdio.h>      // for printf()

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

int main(int argc, char **argv) {
    // Initialize parsec with a program name and description
    parsec_init("bools", "Takes boolean flags and prints their values.");

    // Setup "regular" bool flags
    bool *verbose = parsec_bool_ref("-v", "--verbose", false, "If provided, verbose printing is ON");
    bool *dry_run = parsec_bool_ref(NULL, "--dry-run", false, "If provided, the run will not save any data.");

    // Setup "toggle" bool flags
    bool cov;

    parsec_bool(&cov, NULL, "--cov",   false, "If provided, coverage will be collected");
    parsec_bool(&cov, NULL, "--no-cov", true, "Opposite of --cov");

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    printf("verbose: %s\n", *verbose ? "true" : "false");
    printf("dry-run: %s\n", *dry_run ? "true" : "false");
    printf("cov    : %s\n",  cov     ? "true" : "false");

    return 0;
}
