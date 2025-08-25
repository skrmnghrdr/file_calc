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
    uint64_t UINT;
};

enum operations
{
    ADDITION = 0X01,
    SUBTRACTION = 0X02,
    MULTIPLICATION = 0X03,
    DIVISION = 0X04,
    MODULO = 0X05,
    SHL = 0X06,
    SHR = 0X07,
    AND = 0X08,
    OR = 0X09,
    XOR = 0X0A,
    ROTL = 0X0B,
    ROTR = 0X0C

};
//! use the op code to determine data type

void operations_check();

int process_equation(struct unsolved_equation_t *unsolved_equ, struct solved_equation_t *solved_buffer);

//! shorten, 
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
int solve_equation(uint64_t first_operand, uint8_t operator, uint64_t second_operand, solved_equation_t *solved_buffer);