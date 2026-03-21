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
#include <stddef.h>     // for size_t

#ifndef PARSEC_LIST_CAP
#define PARSEC_LIST_CAP 8
#endif // PARSEC_LIST_CAP

typedef struct {
    char **items;
    size_t len;
    size_t cap;
} ParsecList;

typedef struct {
    char *str;
    size_t len;
} ParsecString;

ParsecList parsec_str_to_list(const char *s, const char *sep);

void parsec_list_free(ParsecList list);

void parsec_string_free(ParsecString s);

void parsec_bool(bool *ref, const char *s, const char *l, bool def, const char *desc);

void parsec_int(int *ref, const char *s, const char *l, int def, const char *desc);

void parsec_float(float *ref, const char *s, const char *l, float def, const char *desc);

void parsec_char(char *ref, const char *s, const char *l, char def, const char *desc);

void parsec_str(char **ref, const char *s, const char *l, char *def, const char *desc);

void parsec_string(ParsecString *ref, const char *s, const char *l, ParsecString def, const char *desc);

void parsec_list(ParsecList *ref, const char *s, const char *l, ParsecList def, const char *desc);

void parsec_help();

void parsec_init(const char *name, const char *desc);

bool parsec_parse(int argc, char** argv);

#endif // PARSEC_H_

#ifdef PARSEC_IMPLEMENTATION

#include <stdlib.h>     // for realloc(), atoi(), strtof(), strtod(), strtoull()
#include <stdio.h>      // for printf()
#include <string.h>     // for strlen(), strcmp()
#include <stdarg.h>     // for va_list

typedef enum {
    PARSEC_BOOL     = 0,
    PARSEC_INT      = 1,
    PARSEC_FLOAT    = 2,
    PARSEC_DOUBLE   = 3,
    PARSEC_SIZE     = 4,
    PARSEC_CHAR     = 5,
    PARSEC_STR      = 6,
    PARSEC_STRING   = 7,
    PARSEC_LIST     = 8,
    // PARSEC_MULTIPLE = 9,
    // PARSEC_ENUM     = 10,
} ParsecType;

typedef union {
    bool _bool;
    int _int;
    float _float;
    double _double;
    size_t _size;
    char _char;
    char *_str;
    ParsecString _string;
    ParsecList _list;
} ParsecValue;

typedef struct {
    char *short_name;
    char *long_name;

    char *desc;
    void *ref;

    ParsecType type;
    ParsecValue def;
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

ParsecFlag *__parsec_add_flag(ParsecContext *ctx, void *ref, const char *s, const char *l, const char *desc, ParsecType type);

char *parsec_str_clone(const char *s);

// Private implementations

ParsecFlag *__parsec_add_flag(ParsecContext *ctx, void *ref, const char *s, const char *l, const char *desc, ParsecType type) {
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

ParsecList parsec_str_to_list(const char *s, const char *sep) {
    char **items = malloc(sizeof(char *) * PARSEC_LIST_CAP);
    if (!items) return (ParsecList){0};

    ParsecList list = {
        .items = items,
        .len = 0,
        .cap = PARSEC_LIST_CAP
    };

    char *tmp = parsec_str_clone(s);

    char *token = strtok(tmp, sep);
    while (token != NULL) {
        if (list.len >= list.cap) {
            list.cap += PARSEC_LIST_CAP;
            list.items = realloc(list.items, list.cap * sizeof(char *));
            if (!list.items) return (ParsecList){0};
        }

        list.items[list.len++] = parsec_str_clone(token);
        token = strtok(NULL, sep);
    }

    free(tmp);

    return list;
}

void parsec_list_free(ParsecList list) {
    for (size_t i = 0; i < list.len; i++) free(list.items[i]);
    free(list.items);
}

void parsec_bool(bool *ref, const char *s, const char *l, bool def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_BOOL);
    flag->def._bool = def;
    *ref = def;
}

void parsec_int(int *ref, const char *s, const char *l, int def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_INT);
    flag->def._int = def;
    *ref = def;
}

void parsec_float(float *ref, const char *s, const char *l, float def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_FLOAT);
    flag->def._float = def;
    *ref = def;
}

void parsec_double(float *ref, const char *s, const char *l, float def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_DOUBLE);
    flag->def._double = def;
    *ref = def;
}

void parsec_size(size_t *ref, const char *s, const char *l, float def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_SIZE);
    flag->def._size = def;
    *ref = def;
}

void parsec_char(char *ref, const char *s, const char *l, char def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_CHAR);
    flag->def._char = def;
    *ref = def;
}

void parsec_str(char **ref, const char *s, const char *l, char *def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_STR);
    flag->def._str = def;
    *ref = def;
}

void parsec_string(ParsecString *ref, const char *s, const char *l, ParsecString def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_STRING);

    flag->def._string = def;
    *ref = def;
}

void parsec_list(ParsecList *ref, const char *s, const char *l, ParsecList def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_LIST);
    flag->def._list = def;
    *ref = def;
}

void parsec_help() {
    printf("%s", parsec.name);
    if (parsec.desc) printf(" - %s", parsec.desc);
    printf("\n\n");

    size_t longest = 0;
    for (size_t i = 0; i < parsec.flags_len; i++) {
        ParsecFlag *flag = parsec.flags[i];

        bool has_long = flag->long_name != NULL && flag->long_name[0] != '\0';
        if (!has_long) continue;

        size_t len = strlen(flag->long_name);
        if (len > longest) longest = len;
    }

    printf("Flags:\n");

    for (size_t i = 0; i < parsec.flags_len; i++) {
        ParsecFlag *flag = parsec.flags[i];

        char *l = flag->long_name;
        char *s = flag->short_name;

        bool has_short = s != NULL && s[0] != '\0';
        bool has_long = l != NULL && l[0] != '\0';

        int offset = has_long ? (int)(longest - strlen(l)) : (int)(longest + strlen(s));
        offset += 2;

        if (has_short && !has_long)      printf("%s%*c%s\n", s,    offset, ' ', flag->desc);
        else if (!has_short && has_long) printf("    %s%*c%s\n", l,    offset, ' ', flag->desc);
        else                             printf("%s, %s%*c%s\n", s, l, offset, ' ', flag->desc);
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

            bool is_short = s != NULL && s[0] != '\0' && strcmp(arg, s) == 0;
            bool is_long = l != NULL && l[0] != '\0' && strcmp(arg, l) == 0;

            if (is_short || is_long) {
                switch (flag->type) {
                case PARSEC_BOOL: {
                    *(bool *)flag->ref = !flag->def._bool;
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

                case PARSEC_DOUBLE: {
                    char *val = parsec_shift(&argc, &argv);

                    char *endptr;
                    *(double *)flag->ref = strtod(val, &endptr);

                    if (*endptr != '\0') PARSEC_THROW(false, "error parsing '%s' to double: %s", arg, val);
                }
                break;

                case PARSEC_SIZE: {
                    char *val = parsec_shift(&argc, &argv);

                    char *endptr;
                    *(size_t *)flag->ref = strtoull(val, &endptr, 10);

                    if (*endptr != '\0') PARSEC_THROW(false, "error parsing '%s' to double: %s", arg, val);
                }
                break;

                case PARSEC_CHAR: {
                    char *val = parsec_shift(&argc, &argv);
                    *(char *)flag->ref = val[0];
                }
                break;

                case PARSEC_STR: {
                    char *val = parsec_shift(&argc, &argv);
                    *(char **)flag->ref = val;
                }
                break;

                case PARSEC_STRING: {
                    char *val = parsec_shift(&argc, &argv);
                    *(ParsecString *)flag->ref = (ParsecString){
                        .str = val,
                        .len = strlen(val)
                    };
                }
                break;

                case PARSEC_LIST: {
                    char *val = parsec_shift(&argc, &argv);

                    ParsecList list = parsec_str_to_list(val, ",");
                    if (!list.items) PARSEC_THROW(false, "error converting to ParsecList '%s'", arg);

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
