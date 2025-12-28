/**
 * @file Arrg.h
 * @brief Lightweight command line argument parser for C
 */

#ifndef ARRG_H
#define ARRG_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Opaque parser structure
 */
typedef struct ar_parser ar_parser;

/**
 * @name Option Flags
 * @brief Flags to configure option behavior. Combine with bitwise OR.
 * @{
 */
#define AR_NO_VAL      0b00000000000000000000000000000110  /**< Option takes no value (flag) */
#define AR_ONE_VAL     0b00000000000000000000000000000010  /**< Option takes exactly one value */
#define AR_MULTI_VAL   0b00000000000000000000000000000100  /**< Option takes multiple values */
#define AR_MANDATORY   0b00000000000000000000000000000001  /**< Option is required */
/** @} */

/**
 * @brief Configuration for a single option
 */
typedef struct ar_conf {
    char sform;         /**< Short form (e.g., 'v' for -v), or '\\0' if none */
    char *lform;        /**< Long form (e.g., "verbose" for --verbose), or NULL if none */
    char *description;  /**< Description shown in help text */
    uint32_t flags;     /**< Bitwise OR of AR_* flags */
} ar_conf;

/**
 * @brief Built-in --help option
 *
 * Add to your configuration array to enable automatic help generation.
 */
extern const ar_conf ARRG_HELP;

/**
 * @brief Built-in --version option
 *
 * Add to your configuration array to enable version display.
 * Set version string with ar_program_version().
 */
extern const ar_conf ARRG_VERSION;

/**
 * @brief Initialize the argument parser
 *
 * @param argc Argument count from main()
 * @param argv Argument vector from main()
 * @param cfgc Number of elements in cfgv
 * @param cfgv Array of option configurations
 * @return Pointer to parser instance, or NULL on failure
 *
 * @note For positional arguments, set both sform='\\0' and lform=NULL
 */
ar_parser *ar_init(int argc, char **argv, int cfgc, ar_conf *cfgv);

/**
 * @brief Parse command line arguments
 *
 * @param parser Parser instance from ar_init()
 * @return 0 on success, non-zero error code on failure
 */
int ar_parse(ar_parser *parser);

/**
 * @brief Free parser resources
 *
 * @param parser Parser instance from ar_init()
 */
void ar_close(ar_parser *parser);

/**
 * @brief Set program version string
 *
 * Used by --version when ARRG_VERSION is in the configuration.
 *
 * @param parser Parser instance
 * @param ver Version string (e.g., "1.0.0")
 */
void ar_program_version(ar_parser *parser, char *ver);

/**
 * @brief Set program name
 *
 * Displayed in usage line of help text.
 *
 * @param parser Parser instance
 * @param name Program name
 */
void ar_program_name(ar_parser *parser, char *name);

/**
 * @brief Set program description
 *
 * Displayed in help text below usage line.
 *
 * @param parser Parser instance
 * @param desc Description string
 */
void ar_program_description(ar_parser *parser, char *desc);

/**
 * @brief Configure error handling behavior
 *
 * @param parser Parser instance
 * @param b If true (default), exit on parse errors. If false, return error code.
 */
void ar_exit_on_error(ar_parser *parser, bool b);

/**
 * @brief Check if an option was provided
 *
 * @param parser Parser instance
 * @param idx Index of option in cfgv array
 * @return true if option was provided, false otherwise
 */
bool ar_is_provided(ar_parser *parser, int idx);

/**
 * @brief Get values for an option
 *
 * @param parser Parser instance
 * @param opt_idx Index of option in cfgv array
 * @return NULL-terminated array of value strings, or NULL if none
 */
char **ar_get_values(ar_parser *parser, int opt_idx);

/**
 * @brief Get number of values for an option
 *
 * @param parser Parser instance
 * @param opt_idx Index of option in cfgv array
 * @return Number of values provided, or 0 if none
 */
int ar_get_val_len(ar_parser *parser, int opt_idx);

#endif
