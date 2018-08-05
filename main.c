#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include "calc.h"

/* Documentation */
static char doc[] =
    "Pi digits calculator using MPFR and two different algorithms to match digits";
static char args_doc[] = "DIGITS";

static struct argp_option options[] = {
    {"output",   'o', "FILE", 0, "Output to FILE instead of standard output"},
    {"threads",   'n', "N", 0, "Use N threads instead of 1"},
    {0}
};

/* Argument parsing */

struct arguments {
    long long unsigned digits;
    unsigned threads;
    char *output_file;
};

static error_t option_parser(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'o':
            arguments->output_file = arg;
        break;

        case 'n':
            arguments->threads = atoi(arg);
        break;

        case ARGP_KEY_ARG:
            if (state->arg_num >= 1) {
            /* too many args */
                argp_usage(state);
            }

            arguments->digits = atoll(arg);
        break;

        case ARGP_KEY_END:
            if (state->arg_num < 1) {
            /* not enough args */
                argp_usage(state);
            }
        break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, option_parser, args_doc, doc};


int main (int argc, char **argv) {
    struct arguments args;
    args.output_file = NULL;
    args.threads = 1;

    argp_parse(&argp, argc, argv, 0, 0, &args);

    FILE *output = stdout;
    if (args.output_file != NULL) {
        output = fopen(args.output_file, "w");

        if (output == NULL) {
            printf("Error\n");
            return -1;
        }
    }

    if (args.digits == 0 || args.threads == 0) {
        printf("Argument invalid\n");
        return -2;
    }

    mpfr_t pi;
    calc_pi(pi, args.digits, args.threads);
    print_pi(output, pi, args.digits);

    return 0;
}