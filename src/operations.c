

#include <stdio.h>
#include <stdlib.h>
#include <operations.h>
#include <string.h>

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
     //test change to see if anything changes on the github
     //make copies and convert to little endian
    int return_value = -1;
     //! structs come with value.
     //! it's already little endian, so we put it to normal to match the equ_viewer.py
    uint32_t equation_id = htobe32(unsolved_equ->equation_id);
    uint64_t first_operand = htobe64(unsolved_equ->ptr_equation.operand_first);
    uint8_t operator = unsolved_equ->ptr_equation.operator;
    uint64_t second_operand = htobe64(unsolved_equ->ptr_equation.operand_second);
    //! one byte deos not need to be endianized
    
    //paranoia 101;
    //! memset(&sovled_buffer.....) kills the pointer
    memset(solved_buffer, 0, sizeof(solved_buffer));
    
    printf("The adjusted endiannes of the things are: \n");
    printf("EquationID: 0x%X 1st: 0x%lX OP:0x%X 2nd: 0x%lX\n",equation_id, first_operand, operator, second_operand);


    //todo start processing things here just deal with endianess and solve
    //pass the value since we don't need things changed, solved_buffer is the output param
    solved_buffer->equation_id = equation_id;
    solve_equation(first_operand, operator, second_operand, &solved_buffer);

    return_value = 0;
    return return_value;

}


int solve_equation(uint64_t first_operand, uint8_t operator, uint64_t second_operand, solved_equation_t *solved_buffer)
{
    /**
     * @brief solves the equation by populating the corresponding
     *        answer on the solved_buffer flag
     * 
     * @calls:
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


    /**
     * 
        //! realized that it came from 8 byte field
        //! so it would definitely fit in a 64 byte int duhhh

        if operator > 0 & operator <= 5
            use int64_t
            operand_first.INT = first_operand //plug that shit brehj
            operand_second.INT = second_operand
        if operator <= 6 & operator <= 12
            use UINT
            operand_first.UINT =  first_operand
            operand_second.UINT = second_operand
        
        switch() 
            add(operand_first.INT, operand_second.UINT);
            ..so on 
            .. so forth
            shower(you)
            workout(you)

        error_code = <whateverop>
        if error, solved_buffer->flags = 0; //set manually down here
        populate solution with nice E770 7777 ERRO RRRR lmaoo
        [equ id][0][0][E770 7777] nice huh
    */


    
    solved_buffer->flags = 0x69;
    solved_buffer->type = 0x69;
    solved_buffer->solution = 0x6998888999999888;
    return_me = 0;
    return return_me;
}