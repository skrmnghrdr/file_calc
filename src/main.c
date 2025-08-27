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
    
    typedef int64_t (*p_ops_t)(int64_t, int64_t, int *);
    typedef uint64_t (*p_uintopts_t)(uint64_t, uint64_t, int *);
    char operations[] = "<<<";
    char *ops[] = {"+", "-", "*", "/","<<", ">>", "<<<", ">>>", "%"};
    int op_ent_len = 9;

    union u_func_union {
        p_ops_t i_func;
        p_uintopts_t u_func;
    };

    typedef enum { 
        RET_INT, RET_UINT 
    } ret_type_t;
    
    typedef struct {
        const char *symbol;
        ret_type_t type;
        union u_func_union func;
    } op_entry;

    op_entry op_entry_arr[] = {
        //    char *ops[] = {"+", "-", "*", "/","<<", ">>", "<<<", ">>>", "%"};
        {"+", RET_INT, .func.i_func = add},
        {"-", RET_INT, .func.i_func = subtract},
        {"*", RET_INT, .func.i_func = multiply},
        {"/", RET_INT, .func.i_func = divide},
        {"<<", RET_UINT, .func.u_func = shift_left},
        {">>", RET_UINT, .func.u_func = shift_right},
        {"<<<", RET_UINT, .func.u_func = rotate_left},
        {">>>", RET_UINT, .func.u_func = rotate_right},
        {"^", RET_UINT, .func.u_func = bitwise_exclusive_or},
    };
    
    for(int idx = 0; idx < op_ent_len; idx++){
        printf("op_entry_arr: op:%s type:%s\n ", op_entry_arr[idx].symbol, op_entry_arr[idx].type == RET_INT ? "int" : "uint");
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
    PRINT_DEBUG("main(): test\n");
    //file_calculator("../laboratory", "../output");
    return 0;
}
