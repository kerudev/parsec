// 10-password-generator/main.c
//
// This example teaches you how to use some simple flags, then uses the values
// for something else than printing.
//
// Usage:
// - ./build/parsec_csv
// - ./build/parsec_csv -p stock.csv -t name,qty,price -l potato,2,3 -l tomato,3,1 -l banana,5,2
//   ./build/parsec_csv -p stock.csv -l carrot,1,1

#include <stdio.h>      // for fopen(), fwrite(), fputc(), fclose(), fprintf(), printf()
#include <sys/stat.h>   // for stat, stat()
#include <string.h>     // for strlen(), strcmp()

#define PARSEC_IMPLEMENTATION
#include "../../parsec.h"

bool path_exists(char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

size_t path_write(char *path, char *header, ParsecList lines) {
    // If the file exists, append. If not, write.
    bool exists = path_exists(path);
    char *modes = (exists) ? "a" : "w";

    FILE *f = fopen(path, modes);

    size_t bytes = 0;

    // Write header if file doesn't exist
    if (!exists) {
        bytes += fwrite(header, sizeof(char), strlen(header), f);
        bytes += fputc('\n', f);
    }

    for (size_t i = 0; i < lines.len; i++) {
        bytes += fwrite(lines.items[i], sizeof(char), strlen(lines.items[i]), f);
        bytes += fputc('\n', f);
    }

    fclose(f);

    return bytes;
}

int main(int argc, char **argv) {
    // Initialize parsec with a program name and description
    parsec_init("csv", "Appends CSV lines to a file.");

    // Setup the flags
    char *path;
    char *header;
    ParsecList lines;

    parsec_str(
        &path, "-p", "--path", "example.csv",
        "Path to the CSV file. If it doesn't exist, it gets created. If it exists, content is appended."
    );
    parsec_str(
        &header, "-t", "--header", "",
        "Headers of the CSV."
    );
    parsec_many(
        &lines, "-l", "--line", (ParsecList){0},
        "Lines to append."
    );

    // Parse the flags
    if (!parsec_parse(argc, argv)) return 1;

    bool exists = path_exists(path);
    if (!exists && strlen(header) == 0) {
        fprintf(stderr, "-t/--header: provide the flags of the file.\n");
        exit(1);
    }

    if (lines.len == 0) {
        fprintf(stderr, "-l/--line: provide at least one line to write.\n");
        exit(1);
    }

    size_t bytes = path_write(path, header, lines);
    printf("Wrote %zu line(s) (%zu bytes) inside %s\n", lines.len, bytes, path);

    // Deallocate the lists
    parsec_list_free(lines);

    return 0;
}
