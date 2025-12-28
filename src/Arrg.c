#include "Arrg.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VAL_MASK 0b00000000000000000000000000000110
#define SPECIAL_V 0b11111111111111111111111111111111
#define SPECIAL_H 0b11111111111111111111111111111110
#define INITIAL_CAPACITY 20

const ar_conf ARRG_HELP = {'\0', "help", "display this help and exit",
    SPECIAL_H};
const ar_conf ARRG_VERSION = {'\0', "version", "display version and exit",
    SPECIAL_V};

typedef enum ret_code {
    SUCCESS,
    NO_VAL,
    TM_VALS,
    TM_POS_ARGS,
    NO_POS_ARGS,      // positional arguments not supported
    MISSING_POS_ARGS, // mandatory positional argument not provided
    MISSING_MAND_VAL, // mandatory value was not provided
    INV_OPT
} ret_code;

typedef struct Array {
    char **items;
    size_t capacity;
    size_t size;
} Array;

typedef struct Values {
    Array *items;
    bool supplied;
} Values;

typedef struct ar_parser {
    int argc;
    char **argv;
    int cfgc;
    ar_conf *cfgv;
    int positional_idx;
    bool default_help;
    bool default_version;
    bool has_options;
    Values *values;
    char *util_name;
    char *util_version;
    char *util_description;
    bool end_of_options;
    bool exit_on_error;
} ar_parser;

static int find_max_lform_size(ar_parser *parser);
static void show_help(ar_parser *parser);
static void print_wrapped(char *text, int offset, int);
static bool lform_equals(char *arg, char *lform);
static bool is_lform(char *arg);
static bool is_sform(char *arg);
static int get_sform_index(ar_parser *parser, char arg);
static int get_lform_index(ar_parser *parser, char *arg);
static void check_ptr(void *ptr);
static void fail(char *reason);
static int add_value(ar_parser *parser, int index, char *value);
static int add_positional(ar_parser *parser, char *arg);
static bool handle_special(ar_parser *parser, char *arg);
static int handle_sform(ar_parser *parser, char *arg, int *arg_idx);
static int handle_lform(ar_parser *parser, char *arg, int *arg_idx);
static bool is_positional(ar_conf);
static Array *da_init();
static void da_add(Array *array, char *str);
static void da_free(Array *array);

ar_parser *ar_init(int argc, char **argv, int cfgc, ar_conf *cfgv) {
    ar_parser *parser = malloc(sizeof(ar_parser));
    check_ptr(parser);
    parser->argc = argc;
    parser->argv = argv;
    parser->cfgc = cfgc;
    parser->cfgv = cfgv;
    parser->positional_idx = -1;
    parser->default_help = false;
    parser->default_version = false;
    parser->has_options = false;
    parser->values = NULL;
    parser->util_name = NULL;
    parser->util_version = NULL;
    parser->util_description = NULL;
    parser->end_of_options = false;
    parser->exit_on_error = true;

    parser->values = malloc(cfgc * sizeof(Values));
    check_ptr(parser->values);
    for (int i = 0; i < cfgc; i++) { // initialize empty arrays
        if (cfgv[i].sform != '\0' || cfgv[i].lform != NULL) {
            parser->has_options = true;
        }
        parser->values[i].items =
            (parser->cfgv[i].flags & VAL_MASK) != AR_NO_VAL ? da_init() : NULL;
        parser->values[i].supplied = false;
        if (parser->cfgv[i].sform == '\0' && parser->cfgv[i].lform == NULL) {
            parser->positional_idx = i;
        }
        if (parser->cfgv[i].flags == SPECIAL_H) {
            parser->default_help = true;
        }
        if (parser->cfgv[i].flags == SPECIAL_V) {
            parser->default_version = true;
        }
    }
    return parser;
}

void ar_exit_on_error(ar_parser *parser, bool b) { parser->exit_on_error = b; }

void ar_program_version(ar_parser *parser, char *ver) {
    parser->util_version = ver;
}

void ar_program_name(ar_parser *parser, char *name) {
    parser->util_name = name;
}

void ar_program_description(ar_parser *parser, char *desc) {
    parser->util_description = desc;
}

bool ar_is_provided(ar_parser *parser, int idx) {
    if (idx >= parser->cfgc)
        return false;
    return parser->values[idx].supplied;
}

char **ar_get_values(ar_parser *parser, int opt_idx) {
    if (opt_idx >= parser->cfgc || parser->values[opt_idx].items == NULL) {
        return NULL;
    }
    return parser->values[opt_idx].items->items;
}

int ar_get_val_len(ar_parser *parser, int opt_idx) {
    if (opt_idx >= parser->cfgc || parser->values[opt_idx].items == NULL) {
        return 0;
    }
    return parser->values[opt_idx].items->size;
}

int ar_parse(ar_parser *parser) {
    for (int i = 1; i < parser->argc; i++) {
        char *arg = parser->argv[i];
        bool result = handle_special(parser, arg);
        if (result)
            continue;
        int ret;
        if (is_lform(arg)) {
            ret = handle_lform(parser, arg, &i);
        } else if (is_sform(arg)) {
            ret = handle_sform(parser, arg, &i);
        } else {
            ret = add_positional(parser, arg);
        }
        if (parser->exit_on_error == false && ret != SUCCESS) {
            return ret;
        }
    }
    for (int i = 0; i < parser->cfgc; i++) {
        if ((parser->cfgv[i].flags & AR_MANDATORY) != 0 &&
                parser->values[i].supplied == false) {
            if (is_positional(parser->cfgv[i])) {
                if (parser->exit_on_error == false) {
                    return MISSING_POS_ARGS;
                }
                fprintf(stderr, "You need to provide at least one %s\n",
                        parser->cfgv[i].description);
            } else {
                if (parser->exit_on_error == false) {
                    return MISSING_MAND_VAL;
                }
                if (parser->cfgv[i].lform == NULL) {
                    fprintf(stderr, "You have to supply at least one value for -%c\n",
                            parser->cfgv[i].sform);
                } else {
                    fprintf(stderr, "You have to supply at least one value for --%s\n",
                            parser->cfgv[i].lform);
                }
            }
            exit(1);
        }
    }
    return SUCCESS;
}

static bool is_positional(ar_conf opt) {
    return (opt.sform == '\0' && opt.lform == NULL);
}

void ar_close(ar_parser *parser) {
    for (int i = 0; i < parser->cfgc; i++) {
        if (parser->values[i].items != NULL) {
            da_free(parser->values[i].items);
        }
    }
    free(parser->values);
    free(parser);
}

static void check_ptr(void *ptr) {
    if (ptr == NULL) {
        fail("memory allocation error");
    }
}

static void fail(char *reason) {
    fprintf(stderr, "%s\n", reason);
    exit(1);
}

static bool handle_special(ar_parser *parser, char *arg) {
    if (parser->end_of_options == true) {
        add_positional(parser, arg);
        return true;
    } else if (strcmp(arg, "--") == 0) {
        parser->end_of_options = true;
        return true;
    } else if (strcmp(arg, "--help") == 0 && parser->default_help == true) {
        show_help(parser);
        exit(0);
    } else if (strcmp(arg, "--version") == 0 && parser->default_version == true) {
        printf("%s v%s\n", parser->util_name, parser->util_version);
        exit(0);
    }
    return false;
}

static int handle_lform(ar_parser *parser, char *arg, int *arg_idx) {
    if (arg == NULL)
        return -1; // critical error, should not occur
    size_t arg_len = strlen(arg);
    int index = get_lform_index(parser, arg);
    if (index == -1) {
        if (parser->exit_on_error == true) {
            fprintf(stderr, "%s is not a valid option\n", arg);
            exit(INV_OPT);
        } else {
            return INV_OPT;
        }
    }
    if ((parser->cfgv[index].flags & VAL_MASK) !=
            AR_NO_VAL) { // option needs at least one value
        int eq_idx = strlen(parser->cfgv[index].lform) +
            2; // length of arg spec+2 and index of = (if exists)
        if (arg[eq_idx] == '=' &&
                arg_len > eq_idx + 1) { // = found followed by more chars
            add_value(parser, index, arg + eq_idx + 1);
        } else if (*arg_idx + 1 < parser->argc) { // value is the next argument
            add_value(parser, index, parser->argv[*arg_idx + 1]);
            (*arg_idx)++;
            return SUCCESS;
        } else {
            if (parser->exit_on_error == true) {
                fprintf(stderr, "Option %s was not supplied a value\n", arg);
                exit(NO_VAL);
            } else {
                return NO_VAL;
            }
        }
    } else {                     // option doesn't accept values
        int argsize = strlen(arg); // size of user supplied argument
        if (argsize > strlen(parser->cfgv[index].lform) + 2) {
            if (parser->exit_on_error == true) {
                fprintf(stderr, "invalid option %s\n", arg);
                exit(INV_OPT);
            } else {
                return INV_OPT;
            }
        }
        parser->values[index].supplied = true;
    }
    return SUCCESS;
}

static int handle_sform(ar_parser *parser, char *arg, int *arg_idx) {
    if (arg == NULL)
        return -1; // critical error, should not occur in the first place
    size_t arg_len = strlen(arg);
    for (size_t j = 1; j < arg_len; j++) { // iterate over all characters
        int index = get_sform_index(parser, arg[j]);
        if (index == -1) {
            if (parser->exit_on_error == true) {
                fprintf(stderr, "-%c is not a valid option\n", arg[j]);
                exit(INV_OPT);
            } else {
                return INV_OPT;
            }
        }
        if ((parser->cfgv[index].flags & VAL_MASK) !=
                AR_NO_VAL) { // needs at least one value.

            if (j == arg_len - 1) {               // value MUST be the next argument
                if (*arg_idx == parser->argc - 1) { // no val supplied
                    if (parser->exit_on_error == true) {
                        fprintf(stderr, "Option %s was not supplied a value\n", arg);
                        exit(NO_VAL);
                    } else {
                        return NO_VAL;
                    }
                } else { // value is next arg
                    add_value(parser, index, parser->argv[*arg_idx + 1]);
                    (*arg_idx)++;
                    return SUCCESS;
                }
            } else { // value is supplied in-place
                add_value(parser, index, arg + j + 1);
                break;
            }
        } else { // doesn't accept values
            parser->values[index].supplied = true;
        }
    }
    return SUCCESS;
}

static int add_positional(ar_parser *parser, char *arg) {
    if (parser->positional_idx == -1) {
        if (parser->exit_on_error == true) {
            fprintf(stderr, "Positional arguments are not supported\n");
            exit(NO_POS_ARGS);
        } else {
            return NO_POS_ARGS;
        }
    }
    int a = add_value(parser, parser->positional_idx, arg);
    return a;
}

static int add_value(ar_parser *parser, int index, char *value) {
    if ((parser->cfgv[index].flags & VAL_MASK) == AR_ONE_VAL) {
        if (parser->values[index].items->size > 0) {
            char *fail_opt = (index == parser->positional_idx)
                ? parser->cfgv[parser->positional_idx].description
                : parser->cfgv[index].lform;
            if (is_positional(parser->cfgv[index])) {
                if (parser->exit_on_error == true) {
                    fprintf(stderr, "Providing more than one %s is not allowed\n",
                            fail_opt);
                    exit(TM_POS_ARGS);
                } else {
                    return TM_POS_ARGS;
                }
            } else {
                if (parser->exit_on_error == true) {
                    fprintf(stderr, "-%c/--%s only accepts a single value\n",
                            parser->cfgv[index].sform, parser->cfgv[index].lform);
                    exit(TM_VALS);
                } else {
                    return TM_VALS;
                }
            }
        }
    }
    da_add(parser->values[index].items, value);
    parser->values[index].supplied = true;
    return SUCCESS;
}

/*
   Accepts a user-supplied lform (e.g. --include) and
   return the index in the spec or -1 if it is
   not found.
   */
static int get_lform_index(ar_parser *parser, char *arg) {
    for (int i = 0; i < parser->cfgc; i++) {
        if (lform_equals(arg, parser->cfgv[i].lform)) {
            return i;
        }
    }
    return -1;
}

static int get_sform_index(ar_parser *parser, char arg) {
    for (int i = 0; i < parser->cfgc; i++) {
        if (parser->cfgv[i].sform == arg) {
            return i;
        }
    }
    return -1;
}

// returns whether the arg is an lform option
static bool is_lform(char *arg) {
    if (arg == NULL)
        return false;
    return strlen(arg) > 2 && strncmp(arg, "--", 2) == 0;
}

static bool is_sform(char *arg) {
    if (arg == NULL)
        return false;
    return strlen(arg) > 1 && strncmp(arg, "-", 1) == 0 &&
        strncmp(arg, "--", 2) != 0;
}

/**
  Checks whether a user supplied argument is equal to
  an lform spec
  */
static bool lform_equals(char *arg, char *lform) {
    if (lform == NULL || arg == NULL)
        return false;
    size_t lform_size = strlen(lform);
    size_t arg_size = strlen(arg);
    if (strncmp(arg, "--", 2) != 0 || arg_size < lform_size + 2) {
        return false;
    } // arg starts with -- and size is reasonable

    for (size_t j = 2; j < lform_size + 2; j++) {
        if (arg[j] != lform[j - 2]) {
            return false;
        }
    } // all characters of the spec match the argument
    if (arg_size > lform_size + 2) {
        return arg[lform_size + 2] == '=';
    } else {
        return true;
    }
}

static void show_help(ar_parser *parser) {
    int max = find_max_lform_size(parser);
    if (parser->util_name != NULL) {
        printf("Usage: %s ", parser->util_name);
        if (parser->has_options == true) {
            fputs("[OPTION]... ", stdout);
        }
        if (parser->positional_idx != -1) {
            printf("[%s]", parser->cfgv[parser->positional_idx].description);
            if ((parser->cfgv[parser->positional_idx].flags & VAL_MASK) !=
                    AR_ONE_VAL) {
                fputs("...", stdout);
            }
        }
        putchar('\n');
    }
    if (parser->util_description != NULL) {
        print_wrapped(parser->util_description, 0, 0);
        putchar('\n');
    }

    for (int i = 0; i < parser->cfgc; i++) {
        char sform = parser->cfgv[i].sform;
        char *lform = parser->cfgv[i].lform;
        if (sform == '\0' && lform == NULL) {
            continue; // positional arguments
        } else if (sform == '\0') {
            printf("      --%s", lform);
        } else {
            printf("  -%c, --%s", sform, lform);
        }
        int len = lform == NULL ? 0 : strlen(lform);
        int offset = max - len + 4; // add 4 spaces
        print_wrapped(parser->cfgv[i].description, max + 8 + 4, offset);
    }
}

static int find_max_lform_size(ar_parser *parser) {
    int size = 0;
    for (int i = 0; i < parser->cfgc; i++) {
        char *l = parser->cfgv[i].lform;
        int len = l == NULL ? 0 : strlen(l);
        if (len > size) {
            size = len;
        }
    }
    return size;
}

static void print_wrapped(char *text, int offset, int first_line_offset) {
    if (text == NULL)
        return;
    for (int i = 0; i < first_line_offset; i++)
        putchar(' ');
    int size = strlen(text);
    int column = offset;
    char ch;
    for (int i = 0; i < size; i++) {
        ch = text[i];
        if ((ch == ' ') && column > 60) { // change line and move to the right
                                          // offset
            putchar('\n');
            for (int j = 0; j < offset; j++)
                putchar(' ');
            column = offset;
        } else {
            putchar(ch);
            column++;
        }
    }
    putchar('\n');
}

static Array *da_init() {
    Array *array = malloc(sizeof(Array));
    char **it = malloc(INITIAL_CAPACITY * sizeof(char *));
    if (array == NULL || it == NULL) {
        fail("memory allocation error");
    }
    array->capacity = INITIAL_CAPACITY;
    array->size = 0;
    array->items = it;
    return array;
}

static void da_add(Array *array, char *str) {
    if (array->size >= array->capacity) {
        array->capacity *= 2;
        char **newPtr = realloc(array->items, array->capacity * sizeof(char *));
        if (newPtr == NULL) {
            fail("memory allocation error");
        }
        array->items = newPtr;
    }
    size_t s = strlen(str) + 1;
    char *word = malloc(s);
    if (word == NULL) {
        fail("memory allocation error");
    }
    memcpy(word, str, s);
    array->items[array->size] = word;
    array->size++;
}

static void da_free(Array *array) {
    for (int i = 0; i < array->size; i++) {
        free(array->items[i]);
    }
    free(array->items);
    free(array);
}
