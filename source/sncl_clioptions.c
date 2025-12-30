#include <sncl_clioptions.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long value;
    bool has_arg;
    char single;
    char *string;
    char *desc;
    char *hint;
} option_t;

struct {
    option_t *options;
    size_t capacity;
    size_t size;
} all_options;

int opt_argc;
int opt_offset;
char *arg_start;
char *const *opt_argv;

int help_spacing;

void opts_push(option_t opt);
char *repeat(char c, int count);

void cliopt_register(unsigned long val, char c, char *str, bool has_arg, char *desc, char *hint) {
    opts_push((option_t){ .value = val, .has_arg = has_arg, .single = c, .string = str, .desc = desc, .hint = hint });
}

void cliopt_start(int argc, char *const *argv) {
    opt_argc = argc;
    opt_argv = argv;
    opt_offset = 1;
}

void cliopt_end() {
    if (all_options.options)
        free(all_options.options);
    all_options.options = NULL;
    all_options.capacity = 0;
    all_options.size = 0;
}

long cliopt_get() {
    if (opt_offset >= opt_argc)
        return CLI_OPT_END_OF_OPTS;

    char *cli_opt = cliopt_str();
    size_t len = strlen(cli_opt);

    int skip = 0;
    if (opt_offset + 1 < opt_argc) {
        // set option as next argument preemptively.
        arg_start = opt_argv[opt_offset + 1];
    } else {
        arg_start = NULL;
    }

    int arg_mode = 0;
    if (len > 1 && cli_opt[0] == '-' && cli_opt[1] == '-') {
        arg_mode = 2;
    } else if (len > 0 && cli_opt[0] == '-') {
        arg_mode = 1;
    }

    if (arg_mode == 2 && len == 2) {
        return CLI_OPT_END_OF_ARGS;
    }

    if (arg_mode == 0) {
        opt_offset++;
        arg_start = cli_opt;
        return CLI_OPT_RAW;
    }

    long res = CLI_OPT_UNKNOWN;
    for (size_t i = 0; i < all_options.size; ++i) {
        option_t *opt = &all_options.options[i];
        if (arg_mode == 1) {
            if (opt->single == '\0')
                continue;
            if (cli_opt[1] == opt->single) {
                if (opt->has_arg)
                    skip = 1;

                if (len > 2) {
                    arg_start = cli_opt + 2;
                    skip = 0;
                }
                res = opt->value;
                break;
            }
        } else if (arg_mode == 2) {
            if (!opt->string)
                continue;
            if (strcmp(cli_opt + 2, opt->string) == 0) {
                if (opt->has_arg)
                    skip = 1;
                res = opt->value;
                break;
            }
        }
    }

    if (res == CLI_OPT_UNKNOWN) {
        arg_start = cli_opt;
    }

    opt_offset += 1 + skip;
    return res;
}

char *cliopt_str() {
    if (opt_offset < opt_argc)
        return opt_argv[opt_offset];
    return NULL;
}

char *cliopt_getarg() { return arg_start; }

int cliopt_idx() { return opt_offset - 1; }

void cliopt_printhelp(char *usage, char *argv0) {
    printf("\n");
    printf("    Usage: %s %s\n", argv0, usage);
    printf("\n");

    int space_count = help_spacing > 0 ? help_spacing : 48;

    printf("Options:\n");
    for (size_t i = 0; i < all_options.size; ++i) {
        option_t *opt = &all_options.options[i];
        if (opt->has_arg) {
            if (opt->single == '\0') {
                char *rep = repeat(' ', space_count - strlen(opt->string) - strlen(opt->hint) - 2 - 2);
                printf("--%s <%s> %s %s\n", opt->string, opt->hint, rep, opt->desc);
                free(rep);
            } else if (opt->string == NULL) {
                char *rep = repeat(' ', space_count - strlen(opt->hint) - 2 - 1 - 1);
                printf("-%c <%s> %s %s\n", opt->single, opt->hint, rep, opt->desc);
                free(rep);
            } else {
                char *rep = repeat(' ', space_count - strlen(opt->string) - strlen(opt->hint) - 2 - 2 - 2 - 1 - 1);
                printf("-%c, --%s <%s> %s %s\n", opt->single, opt->string, opt->hint, rep, opt->desc);
                free(rep);
            }
        } else {
            if (opt->single == '\0') {
                char *rep = repeat(' ', space_count - strlen(opt->string) - 2);
                printf("--%s %s %s\n", opt->string, rep, opt->desc);
                free(rep);
            } else if (opt->string == NULL) {
                char *rep = repeat(' ', space_count - 1 - 1);
                printf("-%c %s %s\n", opt->single, rep, opt->desc);
                free(rep);
            } else {
                char *rep = repeat(' ', space_count - strlen(opt->string) - 2 - 2 - 1 - 1);
                printf("-%c, --%s %s %s\n", opt->single, opt->string, rep, opt->desc);
                free(rep);
            }
        }
    }

    printf("\n");
}

void cliopt_config_help_spacing(int num_spaces) { help_spacing = num_spaces; }

void opts_push(option_t opt) {
    if (all_options.size >= all_options.capacity) {
        all_options.capacity = all_options.capacity == 0 ? 16 : all_options.capacity * 2;
        all_options.options = realloc(all_options.options, all_options.capacity * sizeof(option_t));
    }

    all_options.options[all_options.size++] = opt;
}

char *repeat(char c, int count) {
    char *x = (char *)malloc(sizeof(char) * count);
    memset(x, c, count);
    return x;
}
