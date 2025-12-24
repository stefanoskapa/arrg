#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "Arrg.h"
#define VAL_MASK    0b00000000000000000000000000000110
#define SPECIAL_V   0b11111111111111111111111111111111
#define SPECIAL_H   0b11111111111111111111111111111110
#define INITIAL_CAPACITY 20

const Ar_conf ARRG_HELP = {'\0', "help", "display this help and exit", SPECIAL_H};
const Ar_conf ARRG_VERSION = {'\0', "version", "display version and exit", SPECIAL_V};

static int positional_idx = -1;
static bool default_help = false;
static bool default_version = false;
static bool has_options = false;
static Values *values;
static char *util_name = NULL;
static char *util_version = NULL;
static char *util_description = NULL;
static bool end_of_options = false;
static int values_len = 0;

bool ar_is_provided(int idx) {
    if (idx >= values_len)
        return false;
    return values[idx].supplied;
}

char **ar_get_values(int opt_idx) {
    if (opt_idx >= values_len) {
        return NULL;
    }
    return values[opt_idx].items->items;
}

int ar_get_val_len(int opt_idx) {
    if (opt_idx >= values_len) {
        return 0;
    }
    return values[opt_idx].items->size;
}

int ar_handle(int argc, char *argv[], int cfgc, Ar_conf cfgv[]) {
    init(cfgc, cfgv);
    parse_values(argc, argv, cfgc, cfgv);
    return 0;
}

static void parse_values(int argc, char *argv[], int cfgc, Ar_conf cfgv[]) {
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i]; 
        int arg_len = strlen(arg);
        bool result = handle_special(cfgc,cfgv,arg);
        if (result) continue;

        if (is_lform(arg) ) {       
            result = handle_lform(cfgc,cfgv,argc,argv,arg, arg_len,i);
            if (result) i++;
        } else if (is_sform(arg)) {            
            result = handle_sform(cfgc, cfgv, argc, argv, arg, arg_len, i);
            if (result) i++;
        } else { 
            add_positional(cfgv, arg);
        }
    }
    for (int i = 0; i < cfgc; i++) {
        if ((cfgv[i].flags & AR_MANDATORY) != 0 && values[i].supplied == false) {
            if (is_positional(cfgv[i])) {
                fprintf(stderr, "You need to provide at least one %s\n", cfgv[i].description);
            } else {
                if (cfgv[i].lform == NULL) {
                    fprintf(stderr, "You have to supply at least one value for -%c\n", cfgv[i].sform);
                } else {
                    fprintf(stderr, "You have to supply at least one value for --%s\n", cfgv[i].lform);
                }
            }
            exit(1);
        }
    }
}

static bool is_positional(Ar_conf opt) {
    return (opt.sform == '\0' && opt.lform == NULL);
}
void ar_program_version(char *ver) {
    util_version = ver;
}

void ar_program_name(char *name) {
    util_name = name;
}

void ar_program_description(char *desc) {
    util_description = desc;
}

static void init(int cfgc, Ar_conf cfgv[]) {
    values_len = cfgc;
    values = malloc(cfgc * sizeof(Values));
    check_ptr(values);

    for (int i = 0; i < cfgc; i++) { // initialize empty arrays
        if (cfgv[i].sform != '\0' || cfgv[i].lform != NULL) {
            has_options = true;
        }
        values[i].items = (cfgv[i].flags & VAL_MASK) != AR_NO_VAL ? da_init() : NULL; 
        values[i].supplied = false;
        if (cfgv[i].sform == '\0' && cfgv[i].lform == NULL) {
            positional_idx = i;
        }
        if (cfgv[i].flags == SPECIAL_H) {
            default_help = true;
        }
        if (cfgv[i].flags == SPECIAL_V) {
            default_version = true;
        }
    }
}

void ar_close(int cfgc) {
    for (int i = 0; i < cfgc; i++) {
        if (values[i].items != NULL) {
            da_free(values[i].items);
        }
    }
    free(values);
}

void show_supplied(int cfgc, Ar_conf cfgv[]) {
    for (int i = 0; i < cfgc; i++) {
        if (values[i].supplied) {
            if (cfgv[i].lform == NULL && cfgv[i].sform == '\0') {
                printf("Positional arguments:\n");
            } else {
                printf("--%s\n", cfgv[i].lform);
            }
            if ((cfgv[i].flags & VAL_MASK) != AR_NO_VAL) {
                da_print(*values[i].items);
            }
        }
    }
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

static bool handle_special(int cfgc, Ar_conf *cfgv, char *arg) {
    if (end_of_options == true) {
        add_positional(cfgv,arg);
        return true;
    } else if (strcmp(arg, "--") == 0) {
        end_of_options = true;
        return true;
    } else if (strcmp(arg, "--help") == 0 && default_help == true) {
        show_help(cfgc, cfgv);
        exit(0);
    } else if (strcmp(arg, "--version") == 0 && default_version == true) {
        printf("%s v%s\n", util_name, util_version);
        exit(0);
    }
    return false;
}

// true means increment i
static bool handle_lform(int cfgc, Ar_conf *cfgv,int argc, char *argv[], char *arg, int arg_len, int i) {
    int index = get_lform_index(cfgc, cfgv, arg);
    if (index == -1) {
        fprintf(stderr, "%s is not a valid option\n", arg);
        exit(1);
    }
    if ((cfgv[index].flags & VAL_MASK) != AR_NO_VAL) { // option needs at least one value
        int eq_idx = strlen(cfgv[index].lform) + 2; //length of arg spec+2 and index of = (if exists)
        if (arg[eq_idx] == '=' && arg_len > eq_idx + 1) { // = found followed by more chars 
            int new_val_len = arg_len - eq_idx; // new val len including \0
            char *new_val = malloc(new_val_len); // total arg length - lform+2 = len of value + 1
            check_ptr(new_val);
            memcpy(new_val, arg + eq_idx + 1, new_val_len); 
            add_value(index,cfgv, new_val);
        } else if (i + 1 < argc) { // value is the next argument
            add_value(index, cfgv, argv[i + 1]);
            return true; 
        } else {
            fprintf(stderr, "Option %s was not supplied a value\n", arg);
            exit(1);
        }
    } else { //option doesn't accept values
        int argsize = strlen(arg); //size of user supplied argument
        if (argsize > strlen(cfgv[index].lform) + 2) {
            fprintf(stderr, "invalid option %s\n", arg);
            exit(1);
        }
        values[index].supplied = true;
    }
    return false;
}

static bool handle_sform(int cfgc, Ar_conf *cfgv,int argc, char *argv[], char *arg, int arg_len, int i) {
    for (int j = 1; j < arg_len; j++) { // iterate over all characters 
        int index = get_sform_index(cfgc, cfgv, arg[j]);
        if (index == -1) {
            fprintf(stderr, "-%c is not a valid option\n", arg[j]);
            exit(1);
        }
        if ((cfgv[index].flags & VAL_MASK) != AR_NO_VAL) { //needs at least one value. The value will either be

            if (j == arg_len - 1) {  // value MUST be the next argument
                if (i == argc - 1) { // no val supplied
                    fprintf(stderr, "Option %s was not supplied a value\n", arg);
                    exit(1);
                } else { //value is next arg
                    add_value(index, cfgv, argv[i + 1]);
                    return true; 
                }
            } else { //value is supplied in-place
                char *new_val=malloc(arg_len - j);
                check_ptr(new_val);
                memcpy(new_val, arg + j + 1, arg_len - j);
                add_value(index, cfgv, new_val);
                break;
            }
        } else { //doesn't accept values
            values[index].supplied = true;
        }
    }
    return false;
}

static void add_positional(Ar_conf *cfgv,char *arg) {
    if (positional_idx == -1) {
        fail("Positional arguments are not supported");
    }
    add_value(positional_idx, cfgv, arg);
}

static void add_value(int index, Ar_conf *cfgv, char *value) {
    if ((cfgv[index].flags & VAL_MASK) == AR_ONE_VAL) {
        if (values[index].items->size > 0) {
            if( index == positional_idx) {
                fprintf(stderr, "Supplying more than one %s is not allowed\n", cfgv[positional_idx].description);
            } else {
                fprintf(stderr, "Option %s doesn't accept multiple values\n", cfgv[index].lform);
            }
            exit(1);
        }
    }
    da_add(values[index].items, value); 
    values[index].supplied = true;
}
/*
   Accepts a user-supplied lform (e.g. --include) and
   return the index in the spec or -1 if it is
   not found.
*/
static int get_lform_index(int cfgc, Ar_conf  cfgv[], char *arg) {
    for (int i = 0; i < cfgc; i++) {
        if (lform_equals(arg, cfgv[i].lform)) {
            return i;
        }
    }
    return -1;
}

static int get_sform_index(int cfgc, Ar_conf  cfgv[], char arg) {
    for (int i = 0; i < cfgc; i++) {
        if (cfgv[i].sform == arg) {
            return i;
        }
    }
    return -1;
}

//returns whether the arg is an lform option
static bool is_lform(char *arg) {
    if (arg == NULL) return false;
    return strlen(arg) > 2 && 
        strncmp(arg, "--", 2) == 0;
}

static bool is_sform(char *arg) {
    if (arg == NULL) return false;
    return strlen(arg) > 1 && 
        strncmp(arg, "-", 1) == 0 &&
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

    for (int j = 2; j < lform_size + 2; j++) {
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

static void show_help(int cfgc, Ar_conf cfgv[]) {
    int max = find_max_lform_size(cfgc, cfgv);
    if (util_name != NULL) {
        printf("Usage: %s ", util_name);
        if (has_options == true) {
            fputs("[OPTION]... ", stdout);
        }
        if (positional_idx != -1) {
            printf("[%s]", cfgv[positional_idx].description);
            if ((cfgv[positional_idx].flags & VAL_MASK) != AR_ONE_VAL) {
                fputs("...", stdout);
            }
        }
        putchar('\n');        
    }
    if (util_description != NULL) {
        print_wrapped(util_description, 0, 0);
        putchar('\n');
    }

    for (int i = 0; i < cfgc; i++) {
        char sform = cfgv[i].sform;
        char *lform = cfgv[i].lform;
        if (sform == '\0' && lform == NULL) {
            continue; //positional arguments
        } else if (sform == '\0') {
            printf("      --%s", lform);
        } else {
            printf("  -%c, --%s", sform, lform);
        }
        int len = lform == NULL ? 0 : strlen(lform);
        int offset = max - len + 4; // add 4 spaces
        print_wrapped(cfgv[i].description, max + 8 + 4, offset);
    }
}

static int find_max_lform_size(int cfgc, Ar_conf cfgv[]) {
    int size = 0;
    for (int i = 0; i < cfgc; i++) {
        char *l = cfgv[i].lform; 
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
        if ((ch == ' ') && column > 60) { //change line and move to the right offset
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
    char** it = malloc(INITIAL_CAPACITY * sizeof(char*));
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

static void da_print(Array array) {
    for (int i = 0; i < array.size; i++) {
        printf("%2d: %s\n", i, array.items[i]); 
    }
}

static void da_free(Array *array) {
    for (int i = 0; i < array->size; i++) {
        free(array->items[i]);
    }
    free(array->items);
    free(array);
}
