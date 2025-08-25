

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
    // one byte deos not need to be endianized
    
    //paranoia 101;
    //! memset(&sovled_buffer.....) kills the pointer
    memset(solved_buffer, 0, sizeof(solved_buffer));
    
    printf("[*] EquationID: 0x%X\n1st: 0x%lX\nOP:0x%02X\n2nd: 0x%lX\n",equation_id, first_operand, operator, second_operand);
    //ld or llu
    printf("[*] INT conversion:\n1st:%ld 2nd:%ld\n",(int64_t) first_operand, (int64_t)second_operand);


    //solved_buffer is the output param
    //!solved_buffer is the output param
    //todo solved_buffer is the output param
    //solved_buffer is the output param
    solved_buffer->equation_id = equation_id;
    int solve_result = solve_equation(first_operand, operator, second_operand, solved_buffer);
    if (0 > solve_result)
    {
        printf("[!] operations:process_equation Solve equation something happneed.\n");
        goto END;
    }

    return_value = 0;
END:
    return return_value;
}


int solve_equation(uint64_t first_operand, uint8_t operator, uint64_t second_operand, solved_equation_t *solved_buffer)
{


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
    uint8_t INTEGER_TYPE = 0x01;
    uint8_t UNSIGNED_INTERGER_TYPE = 0x02;
    uint8_t SOLVED = 1;
    uint8_t NOT_SOLVED = 0;
    int ERROR = 0;
    uint8_t hehehe = 0x9;
    solved_buffer->flags = NOT_SOLVED;
    solved_buffer->type = hehehe;
    int calc_error = 0x0; //simp calc uses non POSTIVE val
    
    printf("[^^] GDB anchor here please...\n");

    if( (operator < LOWER_INT_LIMIT) || (operator > HIGHER_UINT_LIMIT) )
    {
        printf("[!] Heresy Detected. Invalid Operator...\n");
        goto END;
    }

    if( (LOWER_INT_LIMIT <= operator ) && (HIGHER_INT_LIMIT >= operator) )
    {
        //! somehow, populatingit here fucks it up, perhaps we should clear buffer?
        //working,. take a rbeak dipshit
        operand_first.INT = first_operand;
        operand_second.INT = second_operand;
        solved_buffer->type = INTEGER_TYPE;
    }

    if( (operator >= LOWER_UINT_LIMIT ) && ( operator <= HIGHER_UINT_LIMIT) )
    {
        operand_first.UINT = first_operand;
        operand_second.UINT = second_operand; 
        solved_buffer->type = UNSIGNED_INTERGER_TYPE;
    }

    PRINT_DEBUG("[*] Processing Operator: 0x%02X\n", operator);
    //! map on c programming
    switch (operator)
    {
    case ADDITION:
        result.INT = add(operand_first.INT, operand_second.INT, &calc_error);
        solved_buffer->type = INTEGER_TYPE;
        break;
    case SUBTRACTION:
        //subtrat seems to work
        result.INT = subtract(operand_first.INT, operand_second.INT, &calc_error);
        break;
    case MULTIPLICATION:
        result.INT = multiply(operand_first.INT, operand_second.INT, &calc_error);
        break;
    case DIVISION:
        //somehow division works as well lol
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
    //todo: consult mentor if bro wants to use the << or our own SHX for overflows
    case ROTL:
        //! fix main calc use the << operator 
        //result.UINT =0xEEEEEEEEEEEE;
        //! test this later
        result.UINT = roate_left(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    case ROTR:
        //! fix main calc, use the << operator 
        result.UINT =0xEEEEEEEEEEEE;
        //result.UINT = rotate_right(operand_first.UINT, operand_second.UINT, &calc_error);
        break;
    default:
        PRINT_DEBUG("[!] Operator showing signs of henneresey..\n");
        calc_error = INVALID_OPERATOR_ERROR;
        break;
    }

    if (calc_error)
    {
        PRINT_DEBUG("[!] operations:solve_equation: Calc error! ewwor code:%d\n", calc_error);
        PRINT_DEBUG("[!] operations:solve_equation:  The chapter master will hear about this..\n");
        goto END;
    }
    
    solved_buffer->solution = result.UINT;
    return_me = 0;
    PRINT_DEBUG("[*]Final result:\nResult[%lX]\n", result.UINT);
    solved_buffer->flags = SOLVED;
    
END:

    return return_me;
}