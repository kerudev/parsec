#define NOB_IMPLEMENTATION
#include "nob.h"

#define CC "gcc"
#define CFLAGS "-std=c99", \
    "-fsanitize=address", "-ggdb", \
    "-Wall", "-Wextra", \
    "-Wno-return-type", \
    "-Wno-discarded-qualifiers"

#define RECIPE(dir, name)                                   \
    do {                                                    \
        Nob_Cmd cmd = {0};                                  \
        nob_cmd_append(&cmd,                                \
            CC, CFLAGS, "-o",                               \
            nob_temp_sprintf("./build/parsec_%s", name),    \
            nob_temp_sprintf("./%s/main.c", dir)            \
        );                                                  \
        if (!nob_cmd_run(&cmd)) return 1;                   \
    } while (0)

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists("build")) return 1;

    RECIPE("00-hello-parsec",           "hello");
    RECIPE("01-bools",                  "bools");
    RECIPE("02-strings",                "strings");
    RECIPE("03-lists",                  "lists");
    RECIPE("10-password-generator",     "password");
    RECIPE("11-csv",                    "csv");

    return 0;
}
