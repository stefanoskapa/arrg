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

typedef struct ar_conf {
    char sform; //short form options, e.g. 'e'
    char *lform; //long form options e.g. 'supress', 'dry-run'
    char *description; // command description for help screen
    uint32_t flags; // use bitwise OR with the AR_* macros
} ar_conf;


/*
 * Optional built-in --help functionality.
 * To use, pass it into the cfgv array.
 */
extern const ar_conf ARRG_HELP;

/*
 * Optional built-in --version functionality.
 * To use, pass it into the cfgv array.
 */
extern const ar_conf ARRG_VERSION;


ar_parser *ar_init(int argc, char **argv, int cfgc, ar_conf *cfgv);
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

void ar_exit_on_error(ar_parser *parser, bool b);
#endif
