# arrg

A minimal, no-nonsense argument parsing library for C.

**arrg** handles the tedious parts of CLI argument parsing so you don't have to. Define your options, call `ar_handle()`, and get on with your program. Invalid input? arrg catches it and exits with a clear error message. Need `--help`? It's built-in.

## Features

- Short (`-v`) and long (`--verbose`) option forms
- Flags, single-value, and multi-value options
- Built-in `--help` and `--version` generation
- Positional argument support
- Mandatory option enforcement
- Automatic error handling for invalid input
- Support for `--` to end option parsing

## Quick Start

```c
#include "arrg.h"

enum { OPT_VERBOSE, OPT_OUTPUT, OPT_HELP };

int main(int argc, char *argv[]) {
    Ar_conf opts[] = {
        {'v', "verbose", "enable verbose output",     AR_NO_VAL},
        {'o', "output",  "output file",               AR_ONE_VAL | AR_MANDATORY},
        ARRG_HELP
    };
    int optc = sizeof(opts) / sizeof(opts[0]);

    ar_program_name("myapp");
    ar_handle(argc, argv, optc, opts);

    if (ar_is_provided(OPT_VERBOSE)) {
        printf("Verbose mode enabled\n");
    }

    char **output = ar_get_values(OPT_OUTPUT);
    printf("Output file: %s\n", output[0]);

    ar_close(optc);
    return 0;
}
```

```
$ ./myapp --output result.txt -v
Verbose mode enabled
Output file: result.txt

$ ./myapp
You have to supply at least one value for --output

$ ./myapp --help
Usage: myapp [OPTION]...
  -v, --verbose    enable verbose output
  -o, --output     output file
      --help       display this help and exit
```

## Installation

Copy `arrg.h`, `arrg.c`, and the dependency `da.h`/`da.c` into your project.

## API Reference

### Configuration

Each option is defined using the `Ar_conf` struct:

```c
typedef struct Ar_conf {
    char     sform;        // short form, e.g. 'v' for -v
    char    *lform;        // long form, e.g. "verbose" for --verbose
    char    *description;  // shown in --help output
    uint32_t flags;        // behavior flags (see below)
} Ar_conf;
```

### Flags

Combine with bitwise OR (`|`):

| Flag | Description |
|------|-------------|
| `AR_NO_VAL` | Option is a flag (no value accepted) |
| `AR_ONE_VAL` | Option accepts exactly one value |
| `AR_MULTI_VAL` | Option accepts multiple values |
| `AR_MANDATORY` | Option must be provided |

### Built-in Options

```c
ARRG_HELP      // adds --help
ARRG_VERSION   // adds --version (set version with ar_program_version())
```

### Functions

```c
// Processing
int ar_handle(int argc, char *argv[], int cfgc, Ar_conf cfgv[]);

// Retrieval (use the option's index in your config array)
bool    ar_is_provided(int opt_idx);
char  **ar_get_values(int opt_idx);
int     ar_get_val_len(int opt_idx);

// Optional setup (call before ar_handle)
void ar_program_name(char *name);
void ar_program_version(char *ver);
void ar_program_description(char *desc);

// Cleanup
void ar_close(int cfgc);
```

## Examples

### Multi-value option

```c
Ar_conf opts[] = {
    {'i', "include", "directories to include", AR_MULTI_VAL},
};
// ...
ar_handle(argc, argv, 1, opts);

int count = ar_get_val_len(0);
char **dirs = ar_get_values(0);
for (int i = 0; i < count; i++) {
    printf("Include: %s\n", dirs[i]);
}
```

```
$ ./myapp -i src -i lib --include=vendor
Include: src
Include: lib
Include: vendor
```

### Positional arguments

Define an option with both `sform` and `lform` set to null:

```c
enum { OPT_FILES };

Ar_conf opts[] = {
    {'\0', NULL, "input files", AR_MULTI_VAL | AR_MANDATORY},
};
```

```
$ ./myapp file1.txt file2.txt
```

### Value syntax

All of these are equivalent:

```
-o output.txt
-ooutput.txt
--output output.txt
--output=output.txt
```

## Error Handling

arrg validates input and exits with a clear message on errors:

```
$ ./myapp --unknown
--unknown is not a valid option

$ ./myapp -o
Option -o was not supplied a value

$ ./myapp --output=one --output=two
Option output doesn't accept multiple values
```

Use `--` to stop option parsing and treat remaining arguments as positional:

```
$ ./myapp -- --this-is-a-filename
```
