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

    //! add the commmit message

    result_t var_result;
    
    for(int idx = 0; idx < op_ent_len; idx++){
        //printf("op_entry_arr: op:%s type:%s\n ", op_entry_arr[idx].symbol, op_entry_arr[idx].type == RET_INT ? "int" : "uint");
        int func_type = op_entry_arr[idx].func_type;
        union _u_func_union ptr_func;  

        int not_equal = strcmp(operation, op_entry_arr[idx].symbol);
        if (not_equal){
            PRINT_DEBUG("[!] Operand not equal, skipping..\n");
            goto END_FOR_LOOP;
        }
        PRINT_DEBUG("[*] Found a match! calculating..\n");

        ptr_func = op_entry_arr[idx].func;
        if (INT_TYPE == func_type && !INVALID_INT_VAL){
            PRINT_DEBUG("[*] processing INT equation..\n");
            var_result.result.i_result = ptr_func.i_func((int64_t) first_operand, (int64_t) second_operand, &calc_error);
            var_result.result_data_type = INT_TYPE;
            goto EXIT_FOR_LOOP;
        }
        if (UINT_TYPE == func_type && !INVALID_UINT_VAL){
            PRINT_DEBUG("[*] processing UINT equation..\n");
            var_result.result.u_result = ptr_func.u_func(first_operand, second_operand, &calc_error);
            var_result.result_data_type = UINT_TYPE;
            goto EXIT_FOR_LOOP;
        }    
END_FOR_LOOP:
    }
EXIT_FOR_LOOP:
    if(INT_TYPE == var_result.result_data_type){
        PRINT_DEBUG("[*] Result %ld\n", var_result.result.i_result);
    }
    if(UINT_TYPE == var_result.result_data_type){
        PRINT_DEBUG("[*] Result %lu\n", var_result.result.i_result);
    }
    //make function pointer, 
    //make the array for the operations, use strings
    //make the function arrray
    //then make the for loop
    //! also see the lookup or hash table please
    /*
    once sometihng matches up, set the operations to that function
     */

    //! try the map function here
    //file_calculator("../laboratory", "../output");
    return 0;
}
