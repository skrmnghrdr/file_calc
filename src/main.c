#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG //! DEBUG HERE
#include "utils.h"
#include "file_calculator.h"
#include "calculator.h"

//! dont forget to clean 
#include "structs.h"
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
int64_t testfunct(int64_t xxx, int64_t yyy)
{   
    int return_value = -1;
    return_value = 0;
    return return_value;
}
int64_t notafunc(int64_t xxx, int64_t yyy)
{   
    int return_value = -1;
    return_value = 0;
    return return_value;
}


int main(int argc, char ** pp_argv)
{
    /**
     * call the solve directory here and ommit the fiile calculator instead of 
     * calling file calc again
     * 
     */


    file_calculator("../laboratory", "../output");
    return 0;
}
