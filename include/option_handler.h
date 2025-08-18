/**
 * @file option_handler.h
 *
 * @brief Option handler for a generic command-line utility.
 */
#ifndef OPTION_HANDLER_H
#define OPTION_HANDLER_H

#include <stdbool.h>
#include <stdint.h> // for int32_t
#include <stddef.h> // for size_t

#define MAX_PATH_LENGTH 255 //repalce back to 256
#define OPT_HANDLER_ERR 1
#define ON_FLAG 1
#define EXIT_FAIL -1

/**
 * @struct options
 * @brief Structure to store command-line options.
 *
 * This structure is used to store the options passed via command-line arguments.
 * It tracks the presence of specific flags and any argument values associated
 * with those options.
 */
typedef struct options_t
{
    bool    f_flag;                     // Flag - input file specified
    char    f_value[MAX_PATH_LENGTH];   // Input file path

    bool    o_flag;                     // Flag - output file specified
    char    o_value[MAX_PATH_LENGTH];   // Output file path

    bool    v_flag;                     // Verbose mode
    bool    d_flag;                     // Debug mode

    bool    h_flag;                     // Help menu requested
} options_t;

/**
 * @brief Processes command-line options and populates an options_t structure.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array containing the command-line arguments.
 * @param options_p Pointer to the options_t structure to populate.
 * @return int - Returns E_SUCCESS (0) on success, E_FAILURE (-1) otherwise.
 */
int process_options(int argc, char ** argv, options_t * options_p);

void show_help(options_t * options_p);
int print_struct(options_t * options_p);

#endif /* _OPTION_HANDLER_H */

/*** end of file ***/

