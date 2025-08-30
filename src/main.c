#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG //! DEBUG HERE
#include "utils.h"
#include "file_calculator.h"
#include "calculator.h"

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
    
    //! fake header
    typedef int64_t (*p_ops_t)(int64_t, int64_t, int *);
    typedef uint64_t (*p_uintopts_t)(uint64_t, uint64_t, int *);
    int INT = 1;
    int UINT = 2;

    union _u_func_union {
        //! do not use alone, bundled with op_entry_t
        p_ops_t i_func;
        p_uintopts_t u_func;
    };

    union _u_result {
        //! do not use alone, bundled with result_t
        uint64_t u_result;
        int64_t i_result;
    };
    typedef struct {
        int result_data_type;
        union _u_result result;
    } result_t;

    typedef struct {
        const char *symbol;
        int func_type;
        union _u_func_union func;
    } op_entry_t;

    op_entry_t op_entry_arr[] = {
        //    char *ops[] = {"+", "-", "*", "/","<<", ">>", "<<<", ">>>", "%"};
        {"+", .func_type=INT,  .func.i_func = add},
        {"-",  .func_type=INT, .func.i_func = subtract},
        {"*", .func_type=INT, .func.i_func = multiply},
        {"/", .func_type=INT, .func.i_func = divide},
        {"<<", .func_type=UINT, .func.u_func = shift_left},
        {">>", .func_type=UINT, .func.u_func = shift_right},
        {"<<<", .func_type=UINT, .func.u_func = rotate_left},
        {">>>", .func_type=UINT, .func.u_func = rotate_right},
        {"^", .func_type=UINT, .func.u_func = bitwise_exclusive_or},
    };
    //! end fake header



    char operation[] = "<<";
    char *ops[] = {"+", "-", "*", "/","<<", ">>", "<<<", ">>>", "%"};
    int op_ent_len = 9;
    int calc_error = 0;
    //! should be in the header

    uint64_t first_operand = 10;
    uint64_t second_operand = 7;
    //! add the commmit message
    int INVALID_UINT_VAL = uint_check_min_max(first_operand, second_operand);
    int INVALID_INT_VAL = int_check_min_max( first_operand, second_operand);

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
        if (INT == func_type && !INVALID_INT_VAL){
            PRINT_DEBUG("[*] processing INT equation..\n");
            var_result.result.i_result = ptr_func.i_func((int64_t) first_operand, (int64_t) second_operand, &calc_error);
            var_result.result_data_type = INT;
            goto EXIT_FOR_LOOP;
        }
        if (UINT == func_type && !INVALID_UINT_VAL){
            PRINT_DEBUG("[*] processing UINT equation..\n");
            var_result.result.u_result = ptr_func.u_func(first_operand, second_operand, &calc_error);
            var_result.result_data_type = UINT;
            goto EXIT_FOR_LOOP;
        }    
END_FOR_LOOP:
    }
EXIT_FOR_LOOP:
    if(INT == var_result.result_data_type){
        PRINT_DEBUG("[*] Result %ld\n", var_result.result.i_result);
    }
    if(UINT == var_result.result_data_type){
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
