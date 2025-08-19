

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
    //! start processing things here
    //! structs come with value.
    //! just deal with endianess and solve
    //! then plug to solved buffer
    //! and make another function to write it on the file
    //! with permissions
    //well what if we just saved this but we did not commit, what would happen
    //! load point anchor over here
    return_value = 0;
    return return_value;
     
}