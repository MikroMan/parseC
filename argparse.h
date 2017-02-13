#ifndef ARGPARSER
#define ARGPARSER

#include <stdbool.h>
#include <stdlib.h>

#define ARG_LIST_LEN 10
#define TOKEN_BUF_SIZE 256

#define PEXIT(s) fprintf(stderr, "ArgParser error: %s\n", s); exit(1);

/*
 * Enumeration for argument value type.
 * Can be integer, double, string, boolean or valueless (only help)
 */
typedef enum Type {
    INT = 0, DOUBLE = 1, BOOL = 2, STRING = 3, NONE = 4
} ArgType;


/*
 * Structure for holding argument data
 * Fields:
 *  short_opt : short option string (specified by user)
 *  long_opt : long option string (specified by user)
 *  required : describes if argument must be present when running program (set by user)
 *  present : describes if argument was set when calling program
 *  v : holds argument value of appropriate type
 *  type : Enum value for determining argument type value (set by user)
 *  description : Short description of argument for help screen (set by user)
 *
 *
 */
typedef struct Arg {
    char *short_opt;
    char *long_opt;
    bool required;
    bool present;
    union value {
        int int_val;
        double d_val;
        bool bool_val;
        char *str_val;
    } v;
    ArgType type;
    char *description;
} Argument;


/*
 * API
 */

/// Initialize argument parser
/// @returns Array of pointers to Argument data
Argument **init_arg_parser();

/// Displays help screen to output
/// @param args Pointer to array of Argument pointers
void print_help(Argument **args);

/// Converts ArgType Enum to string.
/// INT - "INT"
/// DOUBLE - "DOUBLE"
/// INT - "BOOL"
/// STRING - "STRING"
/// NONE - ""
/// @param ArgType Enum value
char *argtype_to_string(ArgType type);

/// Adds argument to argparser list
/// @param args Pointer array for arguments
/// @param short_opt String for short argument option (e.g. h for -h)
/// @param long_opt String for long argument option (e.g. help for --help)
/// @param type ArgType enum value for type of argument (INT, STRING, DOUBLE, BOOL)
/// @param description String describing the argument
/// @param required Boolean value describing whether argument must be present in program call
void add_argument(Argument **args, const char *short_opt, const char *long_opt, ArgType type, const char *description,
                  bool required);

/// Frees memory allocated by Argument Parser
/// @param Array of pointers to Argument structures used in argument parsing
void destroy(Argument **args);

/// Retrieves argument specified by long or short option string from array, if it exists.
/// @param name String representing short or long option.
/// @param args Array of Argument for parsing.
/// @returns Pointer to Argument structure if found, NULL otherwise
Argument *get_arg_by_name(const char *name, Argument **args);

int get_int_val(Argument *arg);

bool get_bool_val(Argument *arg);

char *get_string_val(Argument *arg);

double get_double_val(Argument *arg);

char *get_short_name(Argument *arg);

char *get_long_name(Argument *arg);

void parse_args(Argument **args, int argc, char *argv[]);

/*
 * Error handling and diagnostics
 */
Argument *init_help();

void print_arg_help(Argument *arg);

void print_arg_string(Argument *arg);

void print_arg_value(Argument *arg);

void print_values(Argument **args);

void illegal_access_exit(ArgType actual, ArgType expected);

/*
 * Internal
 */


char *get_name_from_opt(char *opt);

void check_required_opts(Argument **args);

void parse_value(Argument *arg, int argc, int index, char **argv);

char *bool_to_string(bool b);

bool correct_arg(Argument *arg, const char *name);


#endif