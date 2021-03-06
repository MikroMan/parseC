#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argparse.h"

int main(int argc, char *argv[]) {

    Argument **cli_args = init_arg_parser();

    add_argument(cli_args, "fp", "filepath", STRING, NULL, false);
    add_argument(cli_args, "d", NULL, STRING, "Starting date.", false);
    add_argument(cli_args, "k", "kilo", INT, NULL, true);
    add_argument(cli_args, "s", "sort", BOOL, "Sort YES/NO.", false);

    print_help(cli_args);
    parse_args(cli_args, argc, argv);
    print_values(cli_args);

//    Argument *chars_arg = get_arg_by_name("chars", cli_args);
//    int chars = get_int_val(chars_arg);
//    printf("Chars used: %d\n", chars);

    destroy(cli_args);

    return 0;
}
