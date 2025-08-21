#include <stdio.h>
#include <stdlib.h>

#define DEBUG //! DEBUG HERE
#include "utils.h"
#include "file_calculator.h"

//cleanup macro, 


int main(int argc, char ** pp_argv)
{
    //! delete test here for code cave

    


    END: 
    PRINT_DEBUG("main(): test\n");
    file_calculator("../laboratory", "../output");
    return 0;
}

/**
    MOCK CODE FOR THE CLEAN CLEANUP


int func_name()
{

//! #define JANITOR(p) janitor((void**)&(p))
    // Rest of code
    https://stackoverflow.com/questions/6084218/is-it-good-practice-to-free-a-null-pointer-in-c
    From section 7.20.3.2/2 of the C99 standard:\
    #see best answer
    
    int *p_int_array;
    int *p_another_int_arra;
    int *ssomething;

    int* p_int_array = calloc(16u, sizeof(int));
    if(NULL == p_int_array){
        goto CLEANUP;
    }
    int *p_another_int_array = calloc(16U, sizeof(int));
    if(NULL == p_another_int_array){
        goto CLEANUP;
    }
    int *ssomething = calloc(16U, sizeof(int));
    if(NULL == ssomething){
        goto CLEANUP;
    }

CLEANUP:
    JANITOR(p_int_array);
    JANITOR(p_another_int_array);
    JANITOR(ssomething);
return 0;

}
void janitor(void **ptr_dirty)
{
    free(*ptr_dirty);
    *ptr_dirty = NULL;
}

 */