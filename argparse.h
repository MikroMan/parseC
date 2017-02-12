#ifndef ARGPARSER
#define ARGPARSER

#include <stdbool.h>
#include <stdlib.h>

#define ARG_LIST_LEN 10
#define TOKEN_BUF_SIZE 256

#define PEXIT(s) fprintf(stderr, "ArgParser error: %s\n", s); exit(1);


typedef enum Type {
    INT = 0, DOUBLE = 1, BOOL = 2, STRING = 3, NONE = 4
} ArgType;


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

Argument **init_arg_parser();

Argument *init_help();

void print_help(Argument **args);

void print_arg_help(Argument *arg);

char *argtype_to_string(ArgType type);

void add_argument(Argument **args, const char *short_opt, const char *long_opt, ArgType type, const char *description, bool required);


void destroy(Argument **args);

Argument *get_arg_by_name(const char *name, Argument **args);

int get_int_val(Argument *arg);
bool get_bool_val(Argument *arg);
char* get_string_val(Argument *arg);
double get_double_val(Argument *arg);

void illegal_access_exit(ArgType actual, ArgType expected);

void parse_args(Argument **args, int argc, char* argv[]);
void print_values(Argument **args);
void print_arg_string(Argument *arg);
void print_arg_value(Argument* arg);

char* get_name_from_opt(char *opt);
void check_required_opts(Argument **args);
void parse_value(Argument *arg, int argc, int index, char **argv);

char *bool_to_string(bool b);



#endif