// parsec.h - A simple argument parser in C.
//
// To use the implementation, define `PARSEC_IMPLEMENTATION` before including:
//
// #define PARSEC_IMPLEMENTATION
// #include "parsec.h"
//
// Library macros:
// - `PARSEC_DEBUG`: adds debug information on error logs.
//
// Function macros:
// - `PARSEC_THROW`: returns a value and prints an error message.

#ifndef PARSEC_H_
#define PARSEC_H_

#include <stdbool.h>    // for bool

#ifndef PARSEC_LIST_CAP
#define PARSEC_LIST_CAP 8
#endif // PARSEC_LIST_CAP

typedef struct {
    char **items;
    size_t len;
    size_t cap;
} ParsecList;

void parsec_da_free(ParsecList list);

void parsec_bool(bool *ref, const char *s, const char *l, const char *desc);

void parsec_int(int *ref, const char *s, const char *l, const char *desc);

void parsec_float(float *ref, const char *s, const char *l, const char *desc);

void parsec_str(char **ref, const char *s, const char *l, const char *desc);

void parsec_list(ParsecList *ref, const char *s, const char *l, const char *desc);

void parsec_help();

void parsec_init(const char *name, const char *desc);

bool parsec_parse(int argc, char** argv);

#endif // PARSEC_H_

#ifdef PARSEC_IMPLEMENTATION

#include <stddef.h>     // for size_t
#include <stdlib.h>     // for realloc(), atoi()
#include <stdio.h>      // for printf()
#include <string.h>     // for strlen(), strcmp()
#include <stdarg.h>     // for va_list

typedef union {
    bool _bool;
    int _int;
    float _float;
    char *_str;
    ParsecList *_list;
} ParsecValue;

typedef enum {
    PARSEC_BOOL     = 0,
    PARSEC_INT      = 1,
    PARSEC_FLOAT    = 2,
    // PARSEC_DOUBLE   = 3,
    // PARSEC_SIZE     = 4,
    // PARSEC_CHAR     = 5,
    PARSEC_STR      = 6,
    // PARSEC_STRING   = 6,
    PARSEC_LIST     = 8,
    // PARSEC_MULTIPLE = 9,
    // PARSEC_ENUM     = 10,
} ParsecType;

typedef struct {
    char *short_name;
    char *long_name;

    char *desc;
    void *ref;

    ParsecType type;
    ParsecValue value;
} ParsecFlag;

typedef struct {
    char *name;
    char *desc;

    ParsecFlag **flags;
    size_t flags_len;
    size_t flags_cap;
} ParsecContext;

static ParsecContext parsec;

#ifdef PARSEC_DEBUG
static void __parsec_err(const char *file, int line, const char *func, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[%s:%d] %s: ", file, line, func);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#define PARSEC_THROW(ret, fmt, ...) ({ __parsec_err(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); return ret; })
#else
static void __parsec_err(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
#define PARSEC_THROW(ret, fmt, ...) ({ __parsec_err(fmt, ##__VA_ARGS__); return ret; })
#endif

// Private declarations

ParsecFlag *parsec__add_flag(ParsecContext *ctx, void *ref, const char *s, const char *l, const char *desc, ParsecType type);

char *parsec_str_clone(const char *s);

// Private implementations

ParsecFlag *parsec__add_flag(ParsecContext *ctx, void *ref, const char *s, const char *l, const char *desc, ParsecType type) {
    if (ctx->flags_len == ctx->flags_cap) {
        if (ctx->flags_cap == 0) ctx->flags_cap = 4;
        ctx->flags_cap *= 2;

        ctx->flags = realloc(ctx->flags, ctx->flags_cap * sizeof(ParsecFlag *));
    }

    ParsecFlag *flag = malloc(sizeof(ParsecFlag));

    *flag = (ParsecFlag){
        .short_name = s,
        .long_name = l,
        .ref = ref,
        .desc = desc,
        .type = type,
    };

    ctx->flags[ctx->flags_len++] = flag;

    return flag;
}

char *parsec_str_clone(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (!copy) PARSEC_THROW(NULL, "malloc error");

    for (size_t i = 0; i < len; i++) copy[i] = s[i];

    return copy;
}

// Public

void parsec_da_free(ParsecList list) {
    for (size_t i = 0; i < list.len; i++) free(list.items[i]);
    free(list.items);
}

void parsec_bool(bool *ref, const char *s, const char *l, const char *desc) {
    ParsecFlag *flag = parsec__add_flag(&parsec, ref, s, l, desc, PARSEC_BOOL);
    flag->value._bool = false;
}

void parsec_int(int *ref, const char *s, const char *l, const char *desc) {
    ParsecFlag *flag = parsec__add_flag(&parsec, ref, s, l, desc, PARSEC_INT);
    flag->value._int = 0;
}

void parsec_float(float *ref, const char *s, const char *l, const char *desc) {
    ParsecFlag *flag = parsec__add_flag(&parsec, ref, s, l, desc, PARSEC_FLOAT);
    flag->value._float = 0.0f;
}

void parsec_str(char **ref, const char *s, const char *l, const char *desc) {
    ParsecFlag *flag = parsec__add_flag(&parsec, ref, s, l, desc, PARSEC_STR);
    flag->value._str = "";
}

void parsec_list(ParsecList *ref, const char *s, const char *l, const char *desc) {
    parsec__add_flag(&parsec, ref, s, l, desc, PARSEC_LIST);
}

void parsec_help() {
    printf("%s", parsec.name);
    if (parsec.desc) printf(" - %s", parsec.desc);
    printf("\n\n");

    printf("Flags:\n");

    for (size_t i = 0; i < parsec.flags_len; i++) {
        ParsecFlag *flag = parsec.flags[i];

        if (strlen(flag->long_name) == 0) {
            printf("%s        %s\n", flag->short_name, flag->desc);
        }
        else if (strlen(flag->short_name) == 0) {
            printf("    %s   %s\n", flag->long_name, flag->desc);
        }
        else {
            printf("%s, %s   %s\n", flag->short_name, flag->long_name, flag->desc);
        }
    }
}

char *parsec_shift(int *argc, char ***argv) {
    char *arg = **argv;

    *argv += 1;
    *argc -= 1;

    return arg;
}

void parsec_init(const char *name, const char *desc) {
    parsec.name = name;
    parsec.desc = desc;
}

bool parsec_parse(int argc, char** argv) {
    char *program_name = parsec_shift(&argc, &argv);

    if (parsec.name == NULL) {
        parsec.name = program_name;
    }

    while (argc > 0) {
        char *arg = parsec_shift(&argc, &argv);

        for (size_t i = 0; i < parsec.flags_len; i++) {
            ParsecFlag *flag = parsec.flags[i];

            if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
                parsec_help();
                exit(0);
            }

            char *s = flag->short_name;
            char *l = flag->long_name;

            if (strcmp(arg, s) == 0 || strcmp(arg, l) == 0) {
                switch (flag->type) {
                case PARSEC_BOOL: {
                    *(bool *)flag->ref = true;
                }
                break;

                case PARSEC_INT: {
                    char *val = parsec_shift(&argc, &argv);
                    *(int *)flag->ref = atoi(val);
                } 
                break;

                case PARSEC_FLOAT: {
                    char *val = parsec_shift(&argc, &argv);

                    char *endptr;
                    *(float *)flag->ref = strtof(val, &endptr);

                    if (*endptr != '\0') PARSEC_THROW(false, "error parsing '%s' to float: %s", arg, val);
                }
                break;

                case PARSEC_STR: {
                    char *val = parsec_shift(&argc, &argv);
                    *(char **)flag->ref = val;
                }
                break;

                case PARSEC_LIST: {
                    char *val = parsec_shift(&argc, &argv);

                    char **items = malloc(sizeof(char *) * PARSEC_LIST_CAP);
                    if (!items) PARSEC_THROW(false, "malloc error for the argument list '%s'", arg);

                    ParsecList list = {
                        .items = items,
                        .len = 0,
                        .cap = PARSEC_LIST_CAP
                    };

                    char *s = strtok(val, ",");
                    while (s != NULL) {
                        if (list.len >= list.cap) {
                            list.cap += PARSEC_LIST_CAP;
                            list.items = realloc(list.items, list.cap * sizeof(char *));
                            if (!list.items) PARSEC_THROW(false, "realloc error for the argument list '%s'", arg);
                        }

                        list.items[list.len++] = parsec_str_clone(s);
                        s = strtok(NULL, ",");
                    }

                    *(ParsecList *)flag->ref = list;
                }
                break;

                default:
                    exit(1);
                }
            }
        }
    }

    return true;
}

#endif // PARSEC_IMPLEMENTATION
