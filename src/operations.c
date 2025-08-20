

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
    /**
     * @brief: processess the equation, and loads the answer to solved_buffer
     * 
     * @args: 
     *        unsolved_equation_t *unsolved_equ: pointer to the 
     *                             unsolved equation struct
     *        solved_equation_t *solved_equation: pointer to the 
     *                             buffer struct
     * @returns: 
     *         OUTPUT PARAMETER: *solved_buffer
     *         0 for no error
     *         -1 for error
     *
     */
    int return_value = -1;

     //! equ_viewer.py displays it humanly, but process it endian little  
    //htobe64 readable flip human
    uint32_t equation_id = unsolved_equ->equation_id;
    uint64_t first_operand = unsolved_equ->ptr_equation.operand_first;
    uint8_t operator = unsolved_equ->ptr_equation.operator;
    uint64_t second_operand = unsolved_equ->ptr_equation.operand_second;
    //! one byte deos not need to be endianized
    
    //paranoia 101;
    //! memset(&sovled_buffer.....) kills the pointer
    memset(solved_buffer, 0, sizeof(solved_buffer));
    
    printf("The adjusted endiannes of the things are: \n");
    printf("EquationID: 0x%X 1st: 0x%lX OP:0x%X 2nd: 0x%lX\n",equation_id, first_operand, operator, second_operand);


    //solved_buffer is the output param
    //solved_buffer is the output param
    //solved_buffer is the output param
    //solved_buffer is the output param
    solved_buffer->equation_id = equation_id;
    int solve_result = solve_equation(first_operand, operator, second_operand, solved_buffer);
    if (0 > solve_result)
    {
        printf("! Solve equation something happneed.\n");
        goto END;
    }

    return_value = 0;
END:
    return return_value;

}


int solve_equation(uint64_t first_operand, uint8_t operator, uint64_t second_operand, solved_equation_t *solved_buffer)
{
    /**
     * @brief solves the equation by populating the corresponding
     *        answer on the solved_buffer flag
              //! note: operands should be in little endian
     * 
     * @calls:
             //todo populate this
     * 
     * @args: takes in members of the ser_equ_format_t:
     *        see "include/structs.h"
     *        unin64_t first_operand:
     *        uint8_t operator: 0x01 to 0x0C
     *        uint64_t second_operand:
     * 
     * @returns:
             //! OUTPUT PARAMETER: solved_equation_t *solved_buffer
             -1 on error:
             0 on success:
     */  

    int return_me = -1;
    union type_data operand_first;
    union type_data operand_second;
    union type_data result;
    struct results struct_result = {0};

    //spec dictates, fill the magic numbers
    int LOWER_INT_LIMIT = 0x01;
    int HIGHER_INT_LIMIT = 0x05;
    int LOWER_UINT_LIMIT = 0x06;
    int HIGHER_UINT_LIMIT = 0x0C;
    int INTEGER_TYPE = 0x01;
    int UNSIGNED_INTERGER_TYPE = 0x02;
    int SOLVED = 1;
    int NOT_SOLVED = 0;
    int ERROR = 0;
    int hehehe = 0x69E77077;
    solved_buffer->flags = NOT_SOLVED;
    solved_buffer->type = hehehe;
    int calc_error = 0x0; //simp calc uses non POSTIVE val
    
    if( (operator < LOWER_INT_LIMIT) || (operator > HIGHER_UINT_LIMIT) )
    {
        printf("[!] Heresy Detected. Invalid Operator...\n");
        goto END;
    }

    if( (LOWER_INT_LIMIT < operator ) & (HIGHER_INT_LIMIT >= operator) )
    {
        operand_first.INT = first_operand;
        operand_second.INT = second_operand;
        solved_buffer->type = INTEGER_TYPE;
    }

    if( (operator <= LOWER_UINT_LIMIT ) & ( operator <= HIGHER_UINT_LIMIT) )
    {
        operand_first.UINT = first_operand;
        operand_second.UINT = second_operand; 
        solved_buffer->type = UNSIGNED_INTERGER_TYPE;
    }

    printf("[*] Processing Operator: 0x%02X\n", operator);
    switch (operator)
    {
    case ADDITION:
        result.INT = add(operand_first.INT, operand_second.INT, &calc_error);
        break;
    case SUBTRACTION:
        result.INT = subtract(operand_first.INT, operand_second.INT, &calc_error);
        break;
    case MULTIPLICATION:
        result.INT = 0x7FFFFFFFFFFFF;
        result.INT = multiply(operand_first.INT, operand_second.INT, &calc_error);
        break;
    case DIVISION:
        result.INT = divide(operand_first.INT, operand_second.INT, &calc_error);
        break;
    case MODULO:
        result.INT = 0x7FFFFFFFFFFFF;
        //! FYSAss, modulo, you forgot
        //result.INT = modulo(operand_first.INT, operand_second.INT, &calc_error);
        break;
    case SHL:
        result.UINT = shift_left(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    case SHR:
        result.UINT = shift_right(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    case AND:
        result.UINT = bitwise_and(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    case OR:
        result.UINT = bitwise_or(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    case XOR:
        result.UINT = bitwise_exclusive_or(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    case ROTL:
        //! fix main calc
        result.UINT =0xFFFFFFFFFFFF;
        //result.UINT = rotate_left(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    case ROTR:
        //! fix main calc
        result.UINT =0xFFFFFFFFFFFF;
        //result.UINT = rotate_right(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    default:
        printf("[!] Operator showing signs of henneresey..\n");
        calc_error = INVALID_OPERATOR_ERROR;
        break;
    }

    if (calc_error)
    {
        printf("[!] Calc error! ewwor code:%d\n", calc_error);
        printf("[!] The chapter master will hear about this..\n");
        goto END;
    }
    
    solved_buffer->solution = result.UINT;
    return_me = 0;
    printf("[*]Final result:\nResult[%lX]\n", result.UINT);
    solved_buffer->flags = SOLVED;
    
END:

    return return_me;
}