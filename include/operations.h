/**
 * This header is responsible for the mathematical operations/calculations
 * takes input and does the opeartions
 */
//test comment for gitttt

#include "structs.h"
#include <endian.h>
#include <fcntl.h>
#include <unistd.h>

union type_data 
{
    int64_t INT;
    uint64_t U_INT;
};
//! use the op code to determine data type

void operations_check();

int process_equation(struct unsolved_equation_t *unsolved_equ, struct solved_equation_t *solved_buffer);