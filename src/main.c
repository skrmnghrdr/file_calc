#include <stdio.h>
#include <stdlib.h>

#define DEBUG //! DEBUG HERE
#include "utils.h"
#include "file_calculator.h"

//cleanup macro, 

/**
 * @brief main daddy program
 *        informational messages:
 *        [*] regular verbosity
 *        [!] Invalid data
 *        [!!] Critical error
 *        [^^] gdb anchor
 *        [/] task done
 * 
 * @param argc 
 * @param pp_argv 
 * @return int 
 */
int main(int argc, char ** pp_argv)
{
    /**
     * call the solve directory here and ommit the fiile calculator instead of 
     * calling file calc again
     * 
     */

    PRINT_DEBUG("main(): test\n");
    file_calculator("../laboratory", "../output");
    return 0;
}
