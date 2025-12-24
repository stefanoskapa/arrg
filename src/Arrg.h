#ifndef ARRG_H
#define ARRG_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct ar_parser ar_parser;
/**
 * NO_VAL:     used to indicate an option without value (flag)
 * ONE_VAL:    used to indicate an option that accepts one value
 * MULTI_VAL:  used to indicate an option that accepts multiple values
 * MANDATORY:  used to indicate an option that is mandatory
 */
#define AR_NO_VAL      0b00000000000000000000000000000110
#define AR_ONE_VAL     0b00000000000000000000000000000010
#define AR_MULTI_VAL   0b00000000000000000000000000000100
#define AR_MANDATORY   0b00000000000000000000000000000001

/*
 *  USAGE
 *  -----
 *
 *  Stage 1: configuration
 *  This stage involves the necessary and optional
 *  steps before calling handle(). At the very least,
 *  we need to create an array of type map (cfgv) that 
 *  defines the acceptable options for the consuming 
 *  program. Further we can chose to make use and configure 
 *  the built-in functionality for --help and --version.
 *
 *  Stage 2: processing
 *  Here we just need to call handle(), pass our 
 *  cfgv array and its length as well as argc and argv
 *  from main()
 *
 *  Stage 3: value retrieval
 *  After calling handle, the consuming program can retrieve
 *  the parsed values for each option with the help of:
 *
 *    - bool is_provided(int idx);
 *    - char **get_values(int opt_idx);
 *    - int get_val_len(int opt_idx);
 *
 *    where opt_idx is the option index in the cfgv array.
 *
 *  When done using the library, we free up resources by
 *  calling arrg_close().
 */

typedef struct Ar_conf {
    char sform; //short form options, e.g. 'e'
    char *lform; //long form options e.g. 'supress', 'dry-run'
    char *description; // command description for help screen
    uint32_t flags; // use bitwise OR with the AR_* macros
} Ar_conf;

/*
 * Optional built-in --help functionality.
 * To use, pass it into the cfgv array.
 */
extern const Ar_conf ARRG_HELP;

/*
 * Optional built-in --version functionality.
 * To use, pass it into the cfgv array.
 */
extern const Ar_conf ARRG_VERSION;


ar_parser *ar_init(int argc, char **argv, int cfgc, Ar_conf *cfgv);
/**
 * Processes CLI argumets and cfgv configuration
 * Needs to be called before 
 *
 * @param argc argument count from main function
 * @param argv argument array from main function
 * @param cfgc item count in config array
 * @param cfgv config array
 */
void ar_parse(ar_parser *parser);
void show_supplied(int cfgc, Ar_conf cfgv[]);

/**
 * Free's up allocated memory. Should be called
 * when done using arrg.
 */
void ar_close(ar_parser *parser);

/**
 * Set the version number for --version
 * (optional)
 * @param parser pointer to ar_parser object returned from ar_init
 * @param ver Version string
 */
void ar_program_version(ar_parser *parser, char *ver);

/**
 * Provide the program name to 
 * show USAGE within --help
 * (optional)
 * @param parser pointer to ar_parser object returned from ar_init
 * @param name Name of the consuming program
 */
void ar_program_name(ar_parser *parser, char *name);

/**
 * Provide a program description
 * to appear on --help screen.
 * (optional)
 * @param parser pointer to ar_parser object returned from ar_init
 * @param desc A description of the consuming program
 */
void ar_program_description(ar_parser *parser, char *desc);

/**
 * Returns whether the specific option
 * was provided by the user.
 *
 * @param parser pointer to ar_parser object returned from ar_init
 * @param opt_idx Option index in cfgv
 */
bool ar_is_provided(ar_parser *parser, int idx);

/**
 * Returns the provided values for the
 * option as a list of strings.
 *
 * @param parser pointer to ar_parser object returned from ar_init
 * @param opt_idx Option index in cfgv
 */
char **ar_get_values(ar_parser *parser, int opt_idx);

/**
 * Returns how many values the user
 * provided for the specific option.
 *
 * @param parser pointer to ar_parser object returned from ar_init
 * @param opt_idx Option index in cfgv
 */
int ar_get_val_len(ar_parser *parser, int opt_idx);

typedef struct Array {
    char** items;
    size_t capacity;
    size_t size;
} Array;

typedef struct Values {
    Array *items;
    bool supplied;
} Values;

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
static void add_value(ar_parser *parser, int index, char *value);
static void add_positional(ar_parser *parser,char *arg);
static bool handle_special(ar_parser *parser, char *arg);
static bool handle_sform(ar_parser *parser, char *arg, int arg_len, int i);
static bool handle_lform(ar_parser *parser, char *arg, int arg_len, int i);
static bool is_positional(Ar_conf);
//dynamic array
static Array *da_init();
static void da_print(Array array);
static void da_add(Array *array, char *str);
static void da_free(Array *array);
#endif
