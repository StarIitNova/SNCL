/* SNCL CLI Options v1.0
   Defines a simple set of functions to assist in parsing command line arguments similar to how the GNU C Compiler
   might.

   Contributors:
   - StarIitNova (fynotix.dev@gmail.com)
 */

#ifndef SNCL_CLIOPTIONS_H__
#define SNCL_CLIOPTIONS_H__

#include <stdbool.h>

enum {
    CLI_OPT_UNKNOWN = -3,     // Unknown option (unregistered flag)
    CLI_OPT_END_OF_ARGS = -2, // End of arguments reached (`--` was passed)
    CLI_OPT_END_OF_OPTS = -1, // End of options reached (all arguments parsed through `argc`)
    CLI_OPT_FILE = 0,         // An option was passed without the flag designation (no `--` or `-`)
    CLI_OPT_INPUT = 0,        // An option was passed without the flag designation (no `--` or `-`)
    CLI_OPT_RAW = 0           // An option was passed without the flag designation (no `--` or `-`)
};

// Registers an option to the command line.
// If `c` is `'\0'` then it is assumed there is no shorthand alias for this option.
// If `str` is `NULL` then it is assumed there is no named/longform alias for this option.
// If `has_arg` is `true` then getarg will return the following argument
//     (for shorthand alias, it may be directly after the letter without a space).
void cliopt_register(unsigned long val, char c, char *str, bool has_arg, char *desc, char *hint);

// Sets up the cliopt interface for parsing passed arguments. Should be called before cliopt_get.
void cliopt_start(int argc, char *const *argv);
// Cleans up the cliopt interface, freeing all registered options.
void cliopt_end();

// Gets the next argument, returning its argument type
//     (see enum defined in `sncl_clioptions.h`, or passed `val` for cliopt_register).
long cliopt_get();

// Returns the currently indexed string, equivalent to `argv[i]` if iterating in order.
char *cliopt_str();
// Returns the current passed argument.
// Ex for `-Iinclude/directory` it would return `"include/directory"` or for `--out abc` it would return `"abc"`.
char *cliopt_getarg();
// Returns the current index offset for parsing arguments, equivalent to the `i` in `argv[i]`.
int cliopt_idx();

// Prints the help menu, automatically generated from the passed arguments to
// `cliopt_register(_, alias, flag, has_arg, description, hint)`, formatted as
// `-alias, --flag (has_arg ? <hint>) (spacing) description`
void cliopt_printhelp(char *usage, char *argv0);

// Configures the number of spaces used after the flag and alias when logging the help menu.
// Note that the flag, alias, and hint are all considered part of this region to keep an even alignment.
// Default value is `48`.
void cliopt_config_help_spacing(int num_spaces);

#endif // SNCL_CLIOPTIONS_H__
