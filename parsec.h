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

/** Parsec's dynamic array of strings. */
typedef struct {
    /** Elements of the list. */
    char **items;
    /** Number of elements. */
    size_t len;
    /** Max elements the list can hold. */
    size_t cap;
} ParsecList;

/** Parsec's string view. */
typedef struct {
    /** The original string. */
    char *str;
    /** Length of the view. */
    size_t len;
} ParsecString;

/**
 * Returns a `ParsecString` that contains the original string and its length.
 */
ParsecString parsec_str_to_string(const char *s);

/**
 * Splits `s` using `sep`.
 *
 * Returns a `ParsecList` that contains the splitted elements.
 * The list may be empty if memory allocation fails.
 */
ParsecList parsec_str_to_list(const char *s, const char *sep);

/**
 * Pushes `val` to `list`.
 *
 * Returns `true` if the element was pushed, `false` otherwise.
 */
bool parsec_list_push(ParsecList *list, char *val);

/**
 * Frees a `ParsecList`.
 */
void parsec_list_free(ParsecList list);

/**
 * Frees a `ParsecString`.
 */
// void parsec_string_free(ParsecString s);

/**
 * Adds a flag of type `PARSEC_BOOL` to the `parsec` context.
 */
void parsec_bool(bool *ref, const char *s, const char *l, bool def, const char *desc);

/**
 * Adds a flag of type `PARSEC_BOOL` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
bool *parsec_bool_ref(const char *s, const char *l, bool def, const char *desc);

/**
 * Adds a flag of type `PARSEC_INT` to the `parsec` context.
 */
void parsec_int(int *ref, const char *s, const char *l, int def, const char *desc);

/**
 * Adds a flag of type `PARSEC_INT` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
int *parsec_int_ref(const char *s, const char *l, int def, const char *desc);

/**
 * Adds a flag of type `PARSEC_FLOAT` to the `parsec` context.
 */
void parsec_float(float *ref, const char *s, const char *l, float def, const char *desc);

/**
 * Adds a flag of type `PARSEC_FLOAT` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
float *parsec_float_ref(const char *s, const char *l, float def, const char *desc);

/**
 * Adds a flag of type `PARSEC_DOUBLE` to the `parsec` context.
 */
void parsec_double(double *ref, const char *s, const char *l, double def, const char *desc);

/**
 * Adds a flag of type `PARSEC_DOUBLE` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
double *parsec_double_ref(const char *s, const char *l, double def, const char *desc);

/**
 * Adds a flag of type `PARSEC_SIZE` to the `parsec` context.
 */
void parsec_size(size_t *ref, const char *s, const char *l, size_t def, const char *desc);

/**
 * Adds a flag of type `PARSEC_SIZE` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
size_t *parsec_size_ref(const char *s, const char *l, float def, const char *desc);

/**
 * Adds a flag of type `PARSEC_CHAR` to the `parsec` context.
 */
void parsec_char(char *ref, const char *s, const char *l, char def, const char *desc);

/**
 * Adds a flag of type `PARSEC_CHAR` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
char *parsec_char_ref(const char *s, const char *l, char def, const char *desc);

/**
 * Adds a flag of type `PARSEC_STR` to the `parsec` context.
 */
void parsec_str(char **ref, const char *s, const char *l, char *def, const char *desc);

/**
 * Adds a flag of type `PARSEC_STR` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
char **parsec_str_ref( const char *s, const char *l, char *def, const char *desc);

/**
 * Adds a flag of type `PARSEC_STRING` to the `parsec` context.
 */
void parsec_string(ParsecString *ref, const char *s, const char *l, ParsecString def, const char *desc);

/**
 * Adds a flag of type `PARSEC_STRING` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
ParsecString *parsec_string_ref(const char *s, const char *l, ParsecString def, const char *desc);

/**
 * Adds a flag of type `PARSEC_LIST` to the `parsec` context.
 */
void parsec_list(ParsecList *ref, const char *s, const char *l, ParsecList def, const char *desc);

/**
 * Adds a flag of type `PARSEC_LIST` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
ParsecList *parsec_list_ref(const char *s, const char *l, ParsecList def, const char *desc);

/**
 * Adds a flag of type `PARSEC_MANY` to the `parsec` context.
 */
void parsec_many(ParsecList *ref, const char *s, const char *l, ParsecList def, const char *desc);

/**
 * Adds a flag of type `PARSEC_MANY` to the `parsec` context.
 * Returns a reference to the flag's value.
 */
ParsecList *parsec_many_ref(const char *s, const char *l, ParsecList def, const char *desc);

/**
 * Prints the help message.
 */
void parsec_help(bool long_help);

/**
 * Set's the `name` and `description` of your program.
 */
void parsec_init(const char *name, const char *desc);

/**
 * Shifts `argv` by one.
 */
char *parsec_shift(int *argc, char ***argv);

/**
 * Consumes `argc` and `argv` and populates the flag pointers. 
 */
bool parsec_parse(int argc, char** argv);

#endif // PARSEC_H_

#ifdef PARSEC_IMPLEMENTATION

#include <stdlib.h>     // for realloc(), atoi(), strtof(), strtod(), strtoull()
#include <stdio.h>      // for printf()
#include <string.h>     // for strlen(), strcmp()
#include <stdarg.h>     // for va_list

/** Types of flags that are supported. */
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
    PARSEC_MANY     = 9,
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

/** Representation of a flag. */
typedef struct {
    /** Short name. */
    char *short_name;
    /** Long name. */
    char *long_name;
    /** Short text that describes the flag's use. */
    char *desc;

    /** Type that defines how the flag will be parsed. */
    ParsecType type;
    /** Default value. */
    ParsecValue def;

    /** Reference to the value in memory (`NULL` in the `_ref` interface). */
    void *ref;
    /** Value of the flag (used by the `_ref` interface). */
    ParsecValue val;
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

void *__parsec_flag_ref(ParsecFlag *flag);

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

void *__parsec_flag_ref(ParsecFlag *flag) {
    if (flag->ref) return flag->ref;
    return &flag->val;
}

char *parsec_str_clone(const char *s) {
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (!copy) PARSEC_THROW(NULL, "malloc error");

    for (size_t i = 0; i < len; i++) copy[i] = s[i];

    return copy;
}

// Public

ParsecString parsec_str_to_string(const char *s) {
    return (ParsecString){
        .str = s,
        .len = strlen(s)
    };
}

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

bool parsec_list_push(ParsecList *list, char *val) {
    if (list->len >= list->cap) {
        list->cap += PARSEC_LIST_CAP;
        list->items = realloc(list->items, list->cap * sizeof(char *));
        if (!list->items) return false;
    }

    list->items[list->len++] = parsec_str_clone(val);

    return true;
}

void parsec_list_free(ParsecList list) {
    for (size_t i = 0; i < list.len; i++) free(list.items[i]);
    free(list.items);
}

void parsec_bool(bool *ref, const char *s, const char *l, bool def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_BOOL);
    flag->def._bool = def;
    flag->val._bool = def;
    *ref = def;
}

bool *parsec_bool_ref(const char *s, const char *l, bool def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_BOOL);
    flag->def._bool = def;
    flag->val._bool = def;
    return &flag->val._bool;
}

void parsec_int(int *ref, const char *s, const char *l, int def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_INT);
    flag->def._int = def;
    flag->val._int = def;
    *ref = def;
}

int *parsec_int_ref(const char *s, const char *l, int def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_INT);
    flag->def._int = def;
    flag->val._int = def;
    return &flag->val._int;
}

void parsec_float(float *ref, const char *s, const char *l, float def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_FLOAT);
    flag->def._float = def;
    flag->val._float = def;
    *ref = def;
}

float *parsec_float_ref(const char *s, const char *l, float def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_FLOAT);
    flag->def._float = def;
    flag->val._float = def;
    return &flag->val._float;
}

void parsec_double(double *ref, const char *s, const char *l, double def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_DOUBLE);
    flag->def._double = def;
    flag->val._double = def;
    *ref = def;
}

double *parsec_double_ref(const char *s, const char *l, double def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_DOUBLE);
    flag->def._double = def;
    flag->val._double = def;
    return &flag->val._double;
}

void parsec_size(size_t *ref, const char *s, const char *l, size_t def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_SIZE);
    flag->def._size = def;
    flag->val._size = def;
    *ref = def;
}

size_t *parsec_size_ref(const char *s, const char *l, float def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_SIZE);
    flag->def._size = def;
    flag->val._size = def;
    return &flag->val._size;
}

void parsec_char(char *ref, const char *s, const char *l, char def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_CHAR);
    flag->def._char = def;
    flag->val._char = def;
    *ref = def;
}

char *parsec_char_ref(const char *s, const char *l, char def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_CHAR);
    flag->def._char = def;
    flag->val._char = def;
    return &flag->val._char;
}

void parsec_str(char **ref, const char *s, const char *l, char *def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_STR);
    flag->def._str = def;
    flag->val._str = def;
    *ref = def;
}

char **parsec_str_ref(const char *s, const char *l, char *def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_STR);
    flag->def._str = def;
    flag->val._str = def;
    return &flag->val._str;
}

void parsec_string(ParsecString *ref, const char *s, const char *l, ParsecString def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_STRING);
    flag->def._string = def;
    flag->val._string = def;
    *ref = def;
}

ParsecString *parsec_string_ref(const char *s, const char *l, ParsecString def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_STRING);
    flag->def._string = def;
    flag->val._string = def;
    return &flag->val._string;
}

void parsec_list(ParsecList *ref, const char *s, const char *l, ParsecList def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_LIST);
    flag->def._list = def;
    flag->val._list = def;
    *ref = def;
}

ParsecList *parsec_list_ref(const char *s, const char *l, ParsecList def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_LIST);
    flag->def._list = def;
    flag->val._list = def;
    return &flag->val._list;
}

void parsec_many(ParsecList *ref, const char *s, const char *l, ParsecList def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, ref, s, l, desc, PARSEC_MANY);
    flag->def._list = def;
    flag->val._list = def;
    *ref = def;
}

ParsecList *parsec_many_ref(const char *s, const char *l, ParsecList def, const char *desc) {
    ParsecFlag *flag = __parsec_add_flag(&parsec, NULL, s, l, desc, PARSEC_MANY);
    flag->def._list = def;
    flag->val._list = def;
    return &flag->val._list;
}

void parsec_help(bool long_help) {
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

        if      (has_short && !has_long) printf("%s%*c%s\n",     s,    offset, ' ', flag->desc);
        else if (!has_short && has_long) printf("    %s%*c%s\n", l,    offset, ' ', flag->desc);
        else                             printf("%s, %s%*c%s\n", s, l, offset, ' ', flag->desc);

        if (!long_help) continue;

        // longest + strlen(s) + comma + space + 2
        offset = longest + strlen(s) + 1 + 1 + 2;

        switch (flag->type) {
        case PARSEC_BOOL: {
            printf("%*cType: bool   - Default: %d\n", offset, ' ', flag->def._bool);
            break;
        }
        case PARSEC_INT: {
            printf("%*cType: int    - Default: %d\n", offset, ' ', flag->def._int);
            break;
        }
        case PARSEC_FLOAT: {
            printf("%*cType: float  - Default: %f\n", offset, ' ', flag->def._float);
            break;
        }
        case PARSEC_DOUBLE: {
            printf("%*cType: double - Default: %lf\n", offset, ' ', flag->def._double);
            break;
        }
        case PARSEC_SIZE: {
            printf("%*cType: size   - Default: %zu\n", offset, ' ', flag->def._size);
            break;
        }
        case PARSEC_CHAR: {
            printf("%*cType: char   - Default: %c\n", offset, ' ', flag->def._char);
            break;
        }
        case PARSEC_STR: {
            printf("%*cType: char*  - Default: \"%s\"\n", offset, ' ', flag->def._str ? flag->def._str : "");
            break;
        }
        case PARSEC_STRING: {
            printf("%*cType: string - Default: \"%s\"\n", offset, ' ', flag->def._string.len ? flag->def._string.str : "");
            break;
        }
        // case PARSEC_LIST: {
        //     printf("%*cType: list   - Default: %s\n", offset, ' ', flag->def._list);
        //     break;
        // }
        // case PARSEC_MANY: {
        //     printf("%*cType: list   - Default: %s\n", offset, ' ', flag->def._list);
        //     break;
        // }
        default:
            printf("%*cType: char* - Default: %s\n", offset, ' ', flag->def._str);
            break;
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

            if (strcmp(arg, "-h") == 0) {
                parsec_help(false);
                exit(0);
            }

            if (strcmp(arg, "--help") == 0) {
                parsec_help(true);
                exit(0);
            }

            char *s = flag->short_name;
            char *l = flag->long_name;

            bool is_short = s != NULL && s[0] != '\0' && strcmp(arg, s) == 0;
            bool is_long = l != NULL && l[0] != '\0' && strcmp(arg, l) == 0;

            if (is_short || is_long) {
                switch (flag->type) {
                case PARSEC_BOOL: {
                    *(bool *)__parsec_flag_ref(flag) = !flag->def._bool;
                }
                break;

                case PARSEC_INT: {
                    char *val = parsec_shift(&argc, &argv);
                    *(int *)__parsec_flag_ref(flag) = atoi(val);
                }
                break;

                case PARSEC_FLOAT: {
                    char *val = parsec_shift(&argc, &argv);

                    char *endptr;
                    *(float *)__parsec_flag_ref(flag) = strtof(val, &endptr);

                    if (*endptr != '\0') PARSEC_THROW(false, "error parsing '%s' to float: %s", arg, val);
                }
                break;

                case PARSEC_DOUBLE: {
                    char *val = parsec_shift(&argc, &argv);

                    char *endptr;
                    *(double *)__parsec_flag_ref(flag) = strtod(val, &endptr);

                    if (*endptr != '\0') PARSEC_THROW(false, "error parsing '%s' to double: %s", arg, val);
                }
                break;

                case PARSEC_SIZE: {
                    char *val = parsec_shift(&argc, &argv);

                    char *endptr;
                    *(size_t *)__parsec_flag_ref(flag) = strtoull(val, &endptr, 10);

                    if (*endptr != '\0') PARSEC_THROW(false, "error parsing '%s' to double: %s", arg, val);
                }
                break;

                case PARSEC_CHAR: {
                    char *val = parsec_shift(&argc, &argv);
                    *(char *)__parsec_flag_ref(flag) = val[0];
                }
                break;

                case PARSEC_STR: {
                    char *val = parsec_shift(&argc, &argv);
                    *(char **)__parsec_flag_ref(flag) = val;
                }
                break;

                case PARSEC_STRING: {
                    char *val = parsec_shift(&argc, &argv);
                    *(ParsecString *)__parsec_flag_ref(flag) = parsec_str_to_string(val);
                }
                break;

                case PARSEC_LIST: {
                    char *val = parsec_shift(&argc, &argv);

                    ParsecList list = parsec_str_to_list(val, ",");
                    if (!list.cap) PARSEC_THROW(false, "error converting to ParsecList '%s'", arg);

                    *(ParsecList *)__parsec_flag_ref(flag) = list;
                }
                break;

                case PARSEC_MANY: {
                    char *val = parsec_shift(&argc, &argv);
                    parsec_list_push((ParsecList *)__parsec_flag_ref(flag), val);
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
