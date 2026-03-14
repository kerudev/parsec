// parsec.h - A simple argument parser in C.

#ifndef PARSEC_H_
#define PARSEC_H_

#include <stdbool.h>    // for bool
#include <stddef.h>     // for size_t
#include <stdio.h>      // for printf

typedef struct ParsecFlag {
    char short_name;
    char *long_name;
    char *desc;
    void *ref;
} ParsecFlag;

typedef struct ParsecContext {
    ParsecFlag **flags;
    size_t len;
} ParsecContext;

static ParsecContext parsec_ctx;

/**
 * TODO
 */
int parsec__add(void *ref, const char short_name, const char *long_name);

/**
 *TODO
 */  
int parsec_bool(bool *var, const char short_name, const char *long_name, char *desc);

/**
 *TODO
 */  
int parsec_int(int *var, const char short_name, const char *long_name, char *desc);

/**
 *TODO
 */  
int parsec_float(float *var, const char short_name, const char *long_name, char *desc);

/**
 *TODO
 */  
int parsec_str(char **var, const char short_name, const char *long_name, char *desc);

/**
 * TODO
 */
int parsec_help();

/**
 * TODO
 */
int parsec(int argc, char** argv);

#endif // PARSEC_H_

#ifdef PARSEC_IMPLEMENTATION

int parsec__add(void *ref, const char short_name, const char *long_name) {

}

int parsec_bool(bool *var, const char short_name, const char *long_name, char *desc) {
    // parsec__add();
}

int parsec_int(int *var, const char short_name, const char *long_name, char *desc) {

}

int parsec_float(float *var, const char short_name, const char *long_name, char *desc) {

}

int parsec_str(char **var, const char short_name, const char *long_name, char *desc) {

}

int parsec_help() {
    printf("Help:\n");

    for (size_t i = 0; i < parsec_ctx.len; i++) {
        ParsecFlag *flag = parsec_ctx.flags[i];

        if (flag->long_name == NULL) {
            printf("-%c        %s\n", flag->short_name, flag->desc);
        }
        else if (flag->short_name == NULL) {
            printf("    --%s   %s\n", flag->long_name, flag->desc);
        }
        else {
            printf("-%c, --%s   %s\n", flag->short_name, flag->long_name, flag->desc);
        }
    }
}

int parsec(int argc, char** argv) {
    // parsec_ctx = (ParsecContext *)malloc(argc * sizeof(ParsecArg));
}

#endif // PARSEC_IMPLEMENTATION
