/**
 * @file option_handler.c
 * @brief Option Handling template
 *
 * This file contains functions for processing command-line options and printing
 * the help menu. It is responsible for setting up initial configurations based
 * on the options passed to the application.
 */

#include <getopt.h> // getopt()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option_handler.h"


/*================PROTOTYPES==============*/
static void print_help_menu();

/**
 * @brief Report any additional arguments passed to the program.
 *
 * This function is responsible for checking if there are any extra arguments
 * that `getopt` didn't process. If there are, it reports them to the stderr.
 * fprintf(stderr, "Error message, please");
 * @param optind Index of the next element to be processed in argv.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 *
 * @return E_SUCCESS if no additional invalid arguments are found, E_FAILURE
 * otherwise.
 */
static int report_extra_arguments(int optind, int argc, char ** argv);
    /**
     * @brief Reports invalid or incomplete options to stderr.
     *
     * This function is called when an unknown option or an option requiring an
     * argument (like `-n` or `-p`) is passed without one. It prints an appropriate
     * error message to stderr.
     */
    //put optind into a var so we don't modify it

static void report_invalid_options(char invalid_option);

//
// ------------------------------OPTION FUNCTIONS------------------------------
//

/**
 * @brief Process the '-f' command-line option.
 *
 * This function is responsible for processing the '-f' option passed in the
 * command line. The '-f' option specifies the file to read from.
 *
 * @param optarg Pointer to the string containing the argument for the '-f'
 * option.
 * @param options_p Pointer to the options_t struct where the parsed value
 * should be stored.
 *
 * @return 0 on successful processing, -1 otherwise.
 * 
 * @notes: we should proabblyu just put a string here as a placeholder, making 
 *
 * sure the  * string does  not override the buffer. max filename length in lin
 * is 255, so we put the buffer at around 250 just to be safe
 */



static int process_i_option(char * optarg, options_t * options_p){
    /**
     * @brief main program to process the input file
     * @param optarg
     * @param options_p
     * @return 0 on successful processing, -1 otherwise.
     */
    
    int snp_return_val = snprintf(options_p->f_value, sizeof(options_p->f_value), "%s", optarg);
    
    if(options_p->v_flag)
    {
        printf("Verbose: Processing input file....\n");
        printf("Verbose: Writing: %d bytes to  max space of %zu bytes:\n", snp_return_val, sizeof(options_p->f_value));
        if(0 == snp_return_val)
        {
            printf("Verbose:Nothing copied to buffer\n");
        }else if(sizeof(options_p->f_value) != snp_return_val){
            printf("Verbose:Truncating the string, too long\n");
        }
    }
    return 0;
}
static int process_o_option(char * optarg, options_t * options_p){
    /**
     * @brief process options from buffer to value
     * @param options_p
     * @return 0 on successful processing, -1 otherwise.
     */
    int snp_return_val = snprintf(options_p->o_value, sizeof(options_p->o_value), "%s", optarg);
    
    if(options_p->v_flag)
    {
        printf("Verbose: Processing output file....\n");
        printf("Verbose: Writing: %d bytes to  max space of %zu bytes:\n", snp_return_val, sizeof(options_p->o_value));
        if(0 == snp_return_val)
        {
            printf("Verbose:Nothing copied to buffer\n");
        }else if(sizeof(options_p->o_value) != snp_return_val){
            printf("Verbose:Truncating the string, too long\n");
        }
    }
    return 0;
}
static int process_v_option(options_t * options_p){
    /**
     * @brief function will be removed in futher versions
     * @param options_p
     * @return 0 on successful processing, -1 otherwise.
     */
    options_p->v_flag = ON_FLAG;
    printf("Verbose: You just turned on yap mode :)\n");
    return 0;
}
static int process_d_option(options_t * options_p){
    /**
    * @brief [Trainee fills in this doxygen comment]
    * @param options_p
    * @return 0 on successful processing, -1 otherwise.
    */
    options_p->d_flag = ON_FLAG;
    if(options_p->v_flag)
    {
        printf("Verbose:Debugga mode...\n");
    }
    return 0;
}
static int process_h_option(options_t * options_p){
    //h does not take arguemtn, so we'll just print out eht help, I think 
    //even putting an argument on this would be irrelevants
    if(options_p->v_flag)
    {
        printf("Verbose:About to print help, hold on....\n");
    }
    print_help_menu();

    return 0;
}

// +---------------------------------------------------------------------------+
// |                            MAIN OPTION HANDLER                            |
// +---------------------------------------------------------------------------+
int process_options(int argc, char ** argv, options_t * options_p)
{
    /**
     * @brief this is where we would put the switch case to process the 
     * argv from the user, 
     * 
     * //for this testing phase 
     */
    int option;
    int exit_code = EXIT_FAIL;

    /*
    {'test', required_argument, &flagy, &indexy}
    if opt(getopt_long(....));
    if opt == 0 
    */
    //reminder syntax
    //struct <data strcut name> <variable_name> 

    //struct option usage {}
    /*
        const char *name;
        int has_arg; (no_argument, req_argument, ooptional)
        int *flag; if not-null, put the value in this, otherwise return val
        int val;
    */

    /*
    option_handler, how to integrate/ include on your program

    1. initialize a option struct variable
        struct options opts = {0};
    2. Call process_options:
	    process_options(argc, argv, &opts);
        2a. Edit the process options so it will cater to your needs
    3. Optional debug puropses:
	    print_struct(&opts);
    */

    struct option long_options[] = {
        {"help",    no_argument,    NULL, 'h'},
        {"output",  required_argument, NULL, 'o'},
        {"input",  required_argument, NULL, 'i'},        
        {"verbose", no_argument,    NULL, 'v'},
        {"debug",   no_argument,    NULL, 'd'},
        {NULL, 0, NULL, 0}
    };

    while ((option = getopt_long(argc, argv, "hi:o:vd", long_options, NULL)) != -1){
        switch (option)
        {
        case 'v':
            exit_code = process_v_option(options_p);
            break;
        case 'h':
            //prints and exits
            process_h_option(options_p);
            goto END;
        case 'o':
            exit_code = process_o_option(optarg, options_p);
            break;
        case 'i':
            exit_code = process_i_option(optarg, options_p);
            break;
        case 'd':
            exit_code = process_d_option(options_p);
        case 't':
            printf("Test mode coming soon\n");
            break;
        case '?':
            report_invalid_options(optopt);
            
        default:
            //something went wrong if it reached here
            exit_code = EXIT_FAIL;
            goto END;
        }
    }//end while


    int rep_extra_args_error = report_extra_arguments(optind, argc, argv);

FAIL_CHECKPOINT:
    if(EXIT_FAIL == exit_code)
    {
        print_help_menu();
    }

END:
    return 0;
}

// *****************************************************************************
//                          STATIC FUNCTION DEFINITIONS
// *****************************************************************************

static int process_x_option(char * optarg, options_t * options_p)
{
    // Implementation here (replace x with whatever option you're handling)
}

static void report_invalid_options(char invalid_option)
{
    printf("Invalid option %c was provided!, this will be rported\n", invalid_option);
    return;
}

static int report_extra_arguments(int optind, int argc, char ** argv)
{
    int local_optind = optind;
    //put in a local variable to keep local

    for(; local_optind < argc; local_optind++){
        printf("Extra arguments:%s\n", argv[local_optind]);
    }
    return 0;
}

static void print_help_menu()
{
    /**
     * @brief show help function
     * called by process_h or shown when error or no args
     */
    printf("./optionhandler [options]\n\n"
            "Options:\n"
            "\t-h, --help\t\t\t Show this help message and exit\n"
            "\t-o, --output\t\t\t File path for the output file\n"
            "\t-i, --input\t\t\t File path for the input file\n"
            "\t-v, --verbose\t\t\t Verbose mode on. (Start yapping)\n"
            "\t-d, --debug\t\t\t Debug mode on\n"
            "\t-t, --testmode\t\t\t Experimental function\n"
            );
    return;
}
// *****************************************************************************
//                       CUSTOM/EXPERIMENTAL /DBG/FUNCTION DEFINITIONS
// *****************************************************************************


int print_struct(options_t * options_p)
{
    /**
     * @brief for debug purposes or verbose puropses only, print this one
     * as an option
     * 
     * args: struct options_t
     * 
     */
    printf("Printing the contents of the struct\n...");
    printf("f_flag:%d\nf_value:%s\no_flag:%d\no_value:%s\nv_flag%d\nd_flag%d\nh_flag%d\n",
        options_p->f_flag, options_p->f_value,
        options_p->o_flag, options_p->o_value,
        options_p->v_flag, options_p->d_flag,
        options_p->h_flag
            );
    return 0;
}
/*** end of file ***/

