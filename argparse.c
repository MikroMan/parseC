
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "argparse.h"


Argument **init_arg_parser() {
    Argument **args = (Argument **) malloc(ARG_LIST_LEN * sizeof(Argument *));
    memset(args, 0, ARG_LIST_LEN);
    args[0] = init_help();
    return args;
}

Argument *init_help() {
    Argument *help = (Argument *) malloc(sizeof(Argument));
    help->short_opt = (char *) malloc(5 * sizeof(char));
    help->long_opt = (char *) malloc(10 * sizeof(char));
    help->description = (char *) malloc(40 * sizeof(char));

    help->type = NONE;
    help->long_opt = "help";
    help->short_opt = "h";
    help->description = "Displays this help screen";
    help->required = false;
    help->present = false;

    return help;
}

void parse_args(Argument **args, int argc, char *argv[]) {

    for (int i = 1; i < argc; ++i) {
        char *name = get_name_from_opt(argv[i]);
        Argument *arg = get_arg_by_name(name, args);
        if(arg->type == NONE) continue;

        if (arg == NULL) {
            printf("Unknown option: %s\n", argv[i]);
            print_help(args);
            exit(1);
        } else {
            parse_value(arg, argc, i, argv);
            if (arg->type != BOOL) i++;
        }
    }

    check_required_opts(args);
}

void parse_value(Argument *arg, int argc, int index, char **argv) {
    arg->present = true;
    if (arg->type == BOOL) {
        arg->v.bool_val = true;
        return;
    }

    index++;
    if(index >= argc) {
        printf("Option missing value: -%s --%s\n", arg->short_opt, arg->long_opt);
        exit(1);
    }

    if(arg->type == STRING) {
        arg->v.str_val = strdup(argv[index]);
    } else if (arg->type == INT) {
        arg->v.int_val = atoi(argv[index]);
    } else if (arg->type == DOUBLE) {
        arg->v.d_val = atof(argv[index]);
    }
}


void check_required_opts(Argument **args) {
    int i = 0;
    while (i < ARG_LIST_LEN && args[i] != NULL) {
        if (args[i]->required && !(args[i]->present)) {
            printf("Missing argument: -%s --%s\n", args[i]->short_opt, args[i]->long_opt);
            exit(1);
        }
        i++;
    }
}


char *get_name_from_opt(char *opt) {
    if (opt[0] != '-') {
        printf("Illegal option character %c. Expected -. Exiting.\n", opt[0]);
        exit(1);
    } else if (opt[1] == '-') {//long opt
        return opt + 2;
    } else {
        return opt + 1;
    }
}


Argument *get_arg_by_name(const char *name, Argument **args) {
    if (name == NULL) return NULL;

    int i = 0;
    while (i < ARG_LIST_LEN && args[i] != NULL) {
        if (strcmp(args[i]->short_opt, name) == 0 || strcmp(args[i]->long_opt, name) == 0) {
            return args[i];
        }
        i++;
    }

    return NULL;
}

void add_argument(Argument **args, const char *short_opt, const char *long_opt, ArgType type, const char *description,
                  bool required) {
    int i = 0;
    while (args[i] != NULL && i < ARG_LIST_LEN) {
        i++;
    }

    if (i == ARG_LIST_LEN) {
        printf("Implement memory reallocation for more arguments. Exiting.");
        //TODO: realloc, stderr output...
        exit(1);
    }

    if (short_opt == NULL && long_opt == NULL) {
        printf("Positional params not allowed. Must have short OR long option. Exiting.");
        exit(1);
    }

    args[i] = (Argument *) malloc(sizeof(Argument));

    if (short_opt == NULL) {
        args[i]->short_opt = NULL;
    } else {
        args[i]->short_opt = strdup(short_opt);
    }


    if (long_opt == NULL) {
        args[i]->long_opt = NULL;
    } else {
        args[i]->long_opt = strdup(long_opt);
    }

    args[i]->type = type;
    if (description == NULL) {
        args[i]->description = NULL;
    } else {
        args[i]->description = strdup(description);
    }
    args[i]->present = false;
    args[i]->required = required;
    if (type == BOOL) args[i]->v.bool_val = false;
    //TODO add default values

}


/*
 * VALUE GETTERS
 */
int get_int_val(Argument *arg) {
    if (arg == NULL) return 0;
    if (arg->type != INT) {
        illegal_access_exit(arg->type, INT);
    }
    return arg->v.int_val;
}

double get_double_val(Argument *arg) {
    if (arg == NULL) return 0;
    if (arg->type != DOUBLE) {
        illegal_access_exit(arg->type, DOUBLE);
    }
    return arg->v.d_val;
}

char *get_string_val(Argument *arg) {
    if (arg == NULL) return 0;
    if (arg->type != STRING) {
        illegal_access_exit(arg->type, STRING);
    }
    return arg->v.str_val;
}

bool get_bool_val(Argument *arg) {
    if (arg == NULL) return 0;
    if (arg->type != BOOL) {
        illegal_access_exit(arg->type, BOOL);
    }
    return arg->v.bool_val;
}

/*
 * ERRORS
 */
void illegal_access_exit(ArgType actual, ArgType expected) {
    printf("Illegal value access. Expected %s, actual value is %s\n", argtype_to_string(expected),
           argtype_to_string(actual));
    exit(1);
}


/*
 * HELP & OUTPUT
 */

void print_help(Argument **args) {
    printf("Available command line options:\n");
    printf("\t%-5s\t %-10s\t\t\t%-30s\t%s  %s\n", "Short", "Long", "Description", "Required", "Type");
    int i = 0;

    while (args[i] != NULL && i < ARG_LIST_LEN) {

        print_arg_help(args[i]);

        i++;
    }
}

void print_values(Argument **args) {
    printf("[DEBUG] Displaying command line arguments and values.\n");
    int i = 0;

    while (args[i] != NULL && i < ARG_LIST_LEN) {
        print_arg_string(args[i]);
        i++;
    }
}

void print_arg_string(Argument *arg) {

    printf("\t-%-5s/ --%-10s\t: ", arg->short_opt, arg->long_opt);
    print_arg_value(arg);
    printf("\n");
}

void print_arg_value(Argument *arg) {
    switch (arg->type) {
        case INT:
            printf("%d", get_int_val(arg));
            break;
        case DOUBLE:
            printf("%.2f", get_double_val(arg));
            break;
        case BOOL:
            printf("%s", bool_to_string(get_bool_val(arg)));
            break;
        case STRING:
            printf("%s", get_string_val(arg));
            break;
        default:
            return;
    }
}


void print_arg_help(Argument *arg) {
    //TODO format with dynamic lengths!!
    printf("\t-%-5s\t--%-10s\t\t\t%-30s\t%-8s : %s\n", arg->short_opt, arg->long_opt, arg->description, arg->required ? "true" : "false",
           argtype_to_string(arg->type));
}


char *argtype_to_string(ArgType type) {
    switch (type) {
        case INT:
            return "INT";
        case DOUBLE:
            return "DOUBLE";
        case BOOL:
            return "BOOL";
        case STRING:
            return "STRING";
        default:
            return "";
    }
}

void destroy(Argument **args) {
    int i = 0;
    while (i < ARG_LIST_LEN && args[i] != NULL) {

        //TODO figure out why it coredumps???
        //free(args[i]->short_opt);
        //free(args[i]->long_opt);
        //free(args[i]->description);
        free(args[i]);
        i++;
    }
    free(args);

}


char *bool_to_string(bool b) {
    return b ? "true" : "false";
}
