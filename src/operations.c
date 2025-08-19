

#include <stdio.h>
#include <stdlib.h>
#include <operations.h>

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
     * @brief: processess the equation
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
     int return_value = -1;
     //make copies and convert to little endian
     //! structs come with value.
     //! it's already little endian, so we put it to normal to match the equ_viewer.py
    uint32_t equation_id = htobe32(unsolved_equ->equation_id);
    uint64_t first_operand = htobe64(unsolved_equ->ptr_equation.operand_first);
    uint64_t second_operand = htobe64(unsolved_equ->ptr_equation.operand_second);
    //! one byte deos not need to be endianized
    uint8_t operator = unsolved_equ->ptr_equation.operator;
    
    //! start processing things here
    //! just deal with endianess and solve
    //! then plug to solved buffer
    //! and make another function to write it on the file
    //! with permissions
    
    printf("The adjusted endiannes of the things are: \n");
    printf("EquationID:0x%X 1st: 0x%lX OP:0x%X 2nd: 0x%lX\n",equation_id, first_operand, operator, second_operand);
    //! for now, we just get the data and plug it back to write :)

    solved_buffer->equation_id = equation_id;
    solved_buffer->flags = 0x69; //99 temporarry
    solved_buffer->type = 0x69;
    solved_buffer->solution = 0x699888899999999;

    //write this shit to a file somwhere and see
    return_value = 0;
    return return_value;
    /*\
    THIS DOes not even work on vs code ffs.
    how would we put the extension here to work
    */
}