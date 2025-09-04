/**
 * This header is responsible for the mathematical operations/calculations
 * takes input and does the opeartions
 */
//test comment for gitttt

#include "structs.h"
#include <endian.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"


#define LOWER_INT_LIMIT 0x01
#define HIGHER_INT_LIMIT 0x05
#define LOWER_UINT_LIMIT 0x06
#define HIGHER_UINT_LIMIT 0x0C
#define INTEGER_TYPE 0x01
#define UNSIGNED_INTERGER_TYPE 0x02
#define SOLVED 1
#define NOT_SOLVED 0
#define DEBUG

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
int process_equation(struct unsolved_equation_t *unsolved_equ, struct solved_equation_t *solved_buffer);


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



/**
 * @brief gets operation based on the provided operand value
 * 
 * 
 * @args: 
 *       uint8_t operator: the operator code
 *       op_entry_t equation: OUTPUT PARAMETER
 * 
 * @returns:
 *        OUTPUT: equation: the buffer to put the equation
 *        on err: -1
 *        on succ: 0
 *          
 */
int get_operation(uint8_t operator, op_entry_t *equation);


/**
 * @brief HELPER: checks the upper and lower limits of the equation before
 *        we perform the calculations
 * 
 * @args: op_entry_t *equation
 * 
 * @returns:
 *          on err: -1
 *          on succ: 0
 * 
 */
int check_limits(uint64_t first_operand, uint64_t second_operand, op_entry_t *equation);