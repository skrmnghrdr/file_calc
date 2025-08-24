#include <stdio.h>
#include <stdlib.h>

#define DEBUG //! DEBUG HERE
#include "utils.h"
#include "file_calculator.h"

//cleanup macro, 


int main(int argc, char ** pp_argv)
{
    //work with the parameters here please
    PRINT_DEBUG("main(): test\n");
    file_calculator("../laboratory", "../output");
    return 0;
}
