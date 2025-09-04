

#include <stdio.h>
#include <stdlib.h>
#include <operations.h>
#include <string.h>
#include <calculator.h>

void operations_check(){
    /**
     * @brief simple sanity check function to make sure 
     *        we have included the operations.h correctly
     * 
     */
    printf("Operations called and functioning\n");
    return;
}

int process_equation(struct unsolved_equation_t *unsolved_equ, struct solved_equation_t *solved_buffer)
{

    int return_value = -1;
    //! check the null pointers first
     //! equ_viewer.py displays it humanly, but process it endian little  
    //htobe64 readable flip human
    uint32_t equation_id = unsolved_equ->equation_id;
    uint64_t first_operand = unsolved_equ->ptr_equation.operand_first;
    uint8_t operator = unsolved_equ->ptr_equation.operator;
    uint64_t second_operand = unsolved_equ->ptr_equation.operand_second;
    // one byte deos not need to be endianized
    
    //paranoia 101;
    //! memset(&sovled_buffer.....) kills the pointer
    //check the
    memset(solved_buffer, 0, sizeof(solved_buffer));
    
    printf("[*] EquationID: 0x%X\n1st: 0x%lX\nOP:0x%02X\n2nd: 0x%lX\n",equation_id, first_operand, operator, second_operand);
    //ld or llu
    printf("[*] INT conversion:\n1st:%ld 2nd:%ld\n",(int64_t) first_operand, (int64_t)second_operand);

    solved_buffer->equation_id = equation_id;
    int solve_result = solve_equation(first_operand, operator, second_operand, solved_buffer);
    if (0 > solve_result){
        PRINT_DEBUG("[!] operations:process_equation Solve equation something happneed. ticking buffer flags to 0\n");
        solved_buffer->flags = 0;
        solved_buffer->solution = 0xE7707E7707E7707; //ERRORERRORERROR
        goto END;
    }

    return_value = 0;
END:
    return return_value;
}


int solve_equation(uint64_t first_operand, uint8_t operator, uint64_t second_operand, solved_equation_t *solved_buffer)
{
    int return_me = -1;
    if(NULL == solved_buffer){
        PRINT_DEBUG("[!!] Solved buffer null...\n");
        goto END;
    }

    op_entry_t equation;
    int result_get_operation = get_operation(operator, &equation);
    int check_overflow;
    result_t solution;
    solved_buffer->flags = NOT_SOLVED;
    solved_buffer->type;
    int calc_error = 0x0; //simp calc uses non POSTIVE val
    
    if (0 > result_get_operation){
        PRINT_DEBUG("[!!] Error at gettting operations..\n");
        goto END;
    }

    check_overflow = check_limits(first_operand, second_operand, &equation);
    if(0 > check_overflow){
        PRINT_DEBUG("[!!] Check overflow error! ");
        goto END;
    }

    if(INT_TYPE == equation.func_type ){
        solution.result.i_result = equation.func.i_func((int64_t) first_operand,(int64_t) second_operand, &calc_error);
        solution.result_data_type = INT_TYPE;
    }
    if(UINT_TYPE == equation.func_type){
        solution.result.i_result = equation.func.u_func(first_operand, second_operand, &calc_error);
        solution.result_data_type = UINT_TYPE;
    }

    if (calc_error){
        PRINT_DEBUG("[!] operations:solve_equation: Calc error! ewwor code:%d\n", calc_error);
        PRINT_DEBUG("[!] operations:solve_equation:  The chapter master will hear about this..\n");
        goto END;
    }

    if(INT_TYPE == solution.result_data_type ){
        solved_buffer->type = INT_TYPE;
        solved_buffer->solution = solution.result.i_result;
    }
    if(UINT_TYPE == solution.result_data_type){
        solved_buffer->type = UINT_TYPE;
        solved_buffer->solution = solution.result.u_result;
    }

    return_me = 0;
    PRINT_DEBUG("[*]Final result:\nResult[%lX]\n", solved_buffer->solution);
    PRINT_DEBUG("[*]Final INT result:\nResult[%ld]\n", (int64_t) solved_buffer->solution);
    solved_buffer->flags = SOLVED;
END:
    return return_me;
}


int get_operation(uint8_t operator, op_entry_t *equation)
{   
    int return_me = -1;
    if( NULL == equation){
        PRINT_DEBUG("[!!] NULL equation pointer caught..\n");
        goto END;
    }
    int op_ent_len = 11; //! change to 12 when having modulorlolorlorlros
    op_entry_t op_entry_arr[] = {
        //    char *ops[] = {"+", "-", "*", "/","<<", ">>", "<<<", ">>>", "%"};
        //! modulo would throw error here for now, this would be perfect
        //! for testing out the writing errror on function
        {ADDITION, .func_type=INTEGER_TYPE,  .func.i_func = add},
        {SUBTRACTION,  .func_type=INTEGER_TYPE, .func.i_func = subtract},
        {MULTIPLICATION, .func_type=INTEGER_TYPE, .func.i_func = multiply},
        {DIVISION, .func_type=INTEGER_TYPE, .func.i_func = divide},
        {SHL, .func_type=UNSIGNED_INTERGER_TYPE, .func.u_func = shift_left},
        {SHR, .func_type=UNSIGNED_INTERGER_TYPE, .func.u_func = shift_right},
        {ROTL, .func_type=UNSIGNED_INTERGER_TYPE, .func.u_func = rotate_left},
        {ROTR, .func_type=UNSIGNED_INTERGER_TYPE, .func.u_func = rotate_right},
        {XOR, .func_type=UNSIGNED_INTERGER_TYPE, .func.u_func = bitwise_exclusive_or},
        {AND, .func_type=UNSIGNED_INTERGER_TYPE, .func.u_func = bitwise_and},
        {OR, .func_type=UNSIGNED_INTERGER_TYPE, .func.u_func = bitwise_or},
    };

    for(int idx = 0; idx < op_ent_len; idx++){
        if(operator == op_entry_arr[idx].symbol)
        {
            PRINT_DEBUG("[*] Found equation match!..\n");
            *equation = op_entry_arr[idx];
            return_me = 0;
            goto END;
        }
    }

END:
    return return_me;
}

int check_limits(uint64_t first_operand, uint64_t second_operand, op_entry_t *equation)
{
    int INVALID_UINT_VAL = uint_check_min_max(first_operand, second_operand);
    int INVALID_INT_VAL = int_check_min_max( first_operand, second_operand);
    int return_me = -1;

    if(INT_TYPE == equation->func_type && !INVALID_INT_VAL){
        return_me = 0;
        goto END;
    }

    if(UINT_TYPE == equation->func_type && !INVALID_UINT_VAL){
        return_me = 0;
        goto END;
    }
END:
    return return_me;
}