# Arrg

A lightweight, portable, zero-dependency command line argument parser for C.

Arrg is written in standard C99 and works on any platform with a conforming compiler.

## Features

- Short (`-v`) and long (`--verbose`) option formats
- Combined short options (`-abc`)
- Inline values (`-ofile`, `--output=file`)
- Positional arguments
- Multi-value options
- Mandatory option enforcement
- Auto-generated help text
- Built-in `--help` and `--version` support

## Installation

Copy `Arrg.c` and `Arrg.h` into your project, or build as a static library:
```bash
gcc -c Arrg.c -o Arrg.o
ar rcs libarrg.a Arrg.o
```

## Quick Start
```c
#include "Arrg.h"

enum { OPT_VERBOSE, OPT_OUTPUT, OPT_INPUT, OPT_HELP };

ar_conf config[] = {
    {'v', "verbose", "Enable verbose output",  AR_NO_VAL},
    {'o', "output",  "Output file",            AR_ONE_VAL | AR_MANDATORY},
    {'\0', NULL,     "input files",            AR_MULTI_VAL},
    ARRG_HELP
};

int main(int argc, char **argv) {
    ar_parser *p = ar_init(argc, argv, 4, config);
    ar_program_name(p, "mytool");
    ar_program_description(p, "A simple file processor");
    ar_parse(p);

    if (ar_is_provided(p, OPT_VERBOSE)) {
        printf("Verbose mode\n");
    }

    char *output = ar_get_values(p, OPT_OUTPUT)[0];
    printf("Output: %s\n", output);

    int file_count = ar_get_val_len(p, OPT_INPUT);
    char **files = ar_get_values(p, OPT_INPUT);
    for (int i = 0; i < file_count; i++) {
        printf("Input: %s\n", files[i]);
    }

    ar_close(p);
    return 0;
}
```

Running `mytool --help` produces:
```
Usage: mytool [OPTION]... [input files]...
A simple file processor

  -v, --verbose    Enable verbose output
  -o, --output     Output file
      --help       display this help and exit
```

## Configuration

Each option is defined with an `ar_conf` struct:
```c
typedef struct ar_conf {
    char sform;         // Short form: 'v' for -v, or '\0' for none
    char *lform;        // Long form: "verbose" for --verbose, or NULL for none
    char *description;  // Help text description
    uint32_t flags;     // Combination of AR_* flags
} ar_conf;
```

### Flags

| Flag           | Description                          |
|----------------|--------------------------------------|
| `AR_NO_VAL`    | Boolean flag, takes no value         |
| `AR_ONE_VAL`   | Accepts exactly one value            |
| `AR_MULTI_VAL` | Accepts one or more values           |
| `AR_MANDATORY` | Option must be provided              |

Combine flags with bitwise OR:
```c
{'o', "output", "Output file", AR_ONE_VAL | AR_MANDATORY}
```

### Option Types

**Flags** (no value):
```c
{'v', "verbose", "Enable verbose mode", AR_NO_VAL}
// Usage: -v, --verbose
```

**Single value**:
```c
{'o', "output", "Output file", AR_ONE_VAL}
// Usage: -o file, -ofile, --output file, --output=file
```

**Multiple values**:
```c
{'I', "include", "Include paths", AR_MULTI_VAL}
// Usage: -I path1 -I path2, --include=path1 --include=path2
```

### Positional Arguments

Positional arguments are values passed without an option flag, like `mytool file1.txt file2.txt`.

To accept positional arguments, add a configuration entry with both `sform` and `lform` set to null:
```c
{'\0', NULL, "input files", AR_MULTI_VAL}
```

The `description` field is used in the help text to describe what the positional arguments represent.

**Single positional argument:**
```c
{'\0', NULL, "filename", AR_ONE_VAL}
// Usage: mytool config.json
```

**Multiple positional arguments:**
```c
{'\0', NULL, "input files", AR_MULTI_VAL}
// Usage: mytool file1.txt file2.txt file3.txt
```

**Mandatory positional argument:**
```c
{'\0', NULL, "input file", AR_ONE_VAL | AR_MANDATORY}
// Parser will error if no positional argument is provided
```

Retrieve positional arguments using the same functions as regular options:
```c
if (ar_is_provided(p, OPT_INPUT)) {
    int count = ar_get_val_len(p, OPT_INPUT);
    char **files = ar_get_values(p, OPT_INPUT);
    for (int i = 0; i < count; i++) {
        printf("File: %s\n", files[i]);
    }
}
```

**Note:** Only one positional argument configuration is supported per parser.

### Built-in Options

Add these constants to your config array for automatic handling:

| Constant       | Description                                      |
|----------------|--------------------------------------------------|
| `ARRG_HELP`    | Adds `--help`, prints auto-generated help text   |
| `ARRG_VERSION` | Adds `--version`, requires `ar_program_version`  |

Example:
```c
ar_conf config[] = {
    {'v', "verbose", "Enable verbose output", AR_NO_VAL},
    ARRG_HELP,
    ARRG_VERSION
};

ar_parser *p = ar_init(argc, argv, 3, config);
ar_program_name(p, "mytool");
ar_program_version(p, "1.0.0");
ar_parse(p);
```

## API Reference

### Lifecycle
```c
// Initialize parser
ar_parser *ar_init(int argc, char **argv, int cfgc, ar_conf *cfgv);

// Parse arguments (call after ar_init and configuration)
int ar_parse(ar_parser *parser);

// Free resources (call when done)
void ar_close(ar_parser *parser);
```

### Parser Configuration
```c
// Set program name for help text
void ar_program_name(ar_parser *parser, char *name);

// Set program description for help text
void ar_program_description(ar_parser *parser, char *desc);

// Set version string for --version
void ar_program_version(ar_parser *parser, char *ver);

// Control error behavior: true = exit on error (default), false = return error code
void ar_exit_on_error(ar_parser *parser, bool b);
```

### Retrieving Values
```c
// Check if option was provided
bool ar_is_provided(ar_parser *parser, int opt_idx);

// Get array of values for an option
char **ar_get_values(ar_parser *parser, int opt_idx);

// Get number of values for an option
int ar_get_val_len(ar_parser *parser, int opt_idx);
```

## Error Handling

By default, Arrg prints an error message and exits on parse errors. For custom error handling:
```c
ar_parser *p = ar_init(argc, argv, cfgc, config);
ar_exit_on_error(p, false);

int result = ar_parse(p);
if (result != 0) {
    // Handle error
    ar_close(p);
    return 1;
}
```

## Argument Parsing Behavior

- `--` stops option parsing; subsequent arguments are treated as positional
- Short options can be combined: `-abc` is equivalent to `-a -b -c`
- Short options with values can be inline: `-ofile` or separate: `-o file`
- Long options with values can use `=`: `--output=file` or separate: `--output file`
