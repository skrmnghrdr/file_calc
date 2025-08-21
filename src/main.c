#include <stdio.h>
#include <stdlib.h>

#define DEBUG //! DEBUG HERE
#include "utils.h"
#include "file_calculator.h"


int main(int argc, char ** pp_argv)
{

    PRINT_DEBUG("main(): test\n");
    file_calculator("../laboratory", "../output");
    return 0;
}
