/**
 * @file operations.c
 * @author Correy Taylor, taylorswitfty@protonmail.com
 * @brief contains the basic calculator ops
 * @version 0.1
 * @date 2025-07-22
 * 
 * 
 * 
 */


//! you smell that? 
//! some deadcode been here too long it's decomposing
#include <calculator.h>
#include <math.h>
#include <string.h>



int debug_on = 0;
int64_t x = 0;
int64_t y = 0;

int calc_error = 0;
int *ptr_calc_error = &calc_error;



void debug(){
    /* debug purposes only, used to achor in gdb */
    return;
}

void usage(){
    // do not print this usage, but due to program stability,
    // we shall preserve this func 😱
    /*
    printf("./simplecalc <argument1> <opearand> <argument2>\n");
    printf("example usage:\n./simplecalc 1 + 1\n");
    printf("./simplecalc 2 * 5\n");
    */
    return;
}

int int_check_min_max(uint64_t xxx, uint64_t yyy){
    /**
     * @brief checks xxx and yyy if they're both legal values of int32_max
     * we initally put long since strtol already has error checkin if we provide a long-er value than long
     * we then do another checking inside the operand function
     * we will write a separate uint checker for our anxiety to 
     * calm down and not rely on the operand.
     */
    if(
        (xxx > INT64_MAX) || 
        (xxx < INT64_MIN) ||
        (yyy > INT64_MAX) ||
        (yyy < INT64_MIN)  )
    {
        return 1;
    }
    return 0;
}

int uint_check_min_max(uint64_t xxx, uint64_t yyy){
    /** @brief samesies with int, just uint */
    if(
        (xxx > UINT64_MAX) || 
        (xxx < UINT64_MIN) ||
        (yyy > UINT64_MAX) ||
        (yyy < UINT64_MIN)  )
    {
        return 1;
    }
    return 0;
}


int64_t add(int64_t xxx, int64_t yyy, int *calc_error){
    /**
     * @brief adding two numbers with over and underflow checks
     * extern int calc_error returns error code
     */
    //check_int32(xxx, yyy); 
    //would work here as well, 
    //print the error
    //return exit code to user pass/fail
    int64_t result = 0;
    *calc_error = 0;

    if(
        //overflow check
        ((xxx > 0) && (yyy > (INT64_MAX - xxx))) ||
        //underflow check
        ((xxx < 0 ) && (yyy < (INT64_MIN - xxx)))
    )
    {
        debug();
        *calc_error = ADDITION_ERROR;
        return result;
    }
    else
    {   
        result = (xxx + yyy);
        debug();
        return result;
    }
}

int64_t subtract(int64_t xxx, int64_t yyy, int *calc_error){
     int64_t result = 0;
     *calc_error = 0;
    if(
        //overflow check
        ((xxx < 0) && (yyy > (INT64_MAX + xxx))) ||
        //underflow check
        ((xxx > 0 ) && (yyy < (INT64_MIN + xxx)))
        //alright, took some time in this, but umm yeah
    )
    {
        debug();
        *calc_error = SUBTRACTION_ERROR;
        return result;
    }
    else
    {   
        result = (xxx - yyy);
        debug();
        return result;
    }
}

int64_t multiply(int64_t xxx, int64_t yyy, int *calc_error){

    int64_t result = 0;
    *calc_error = 0;
    //do not forget to check for 0 since we are diving here
    
    //handle special bad cases here
    if( (0 == yyy || 0 == xxx )){
        goto MUL_END;
    }
    if ((xxx == INT64_MIN && yyy == -1) || (yyy == INT64_MIN && xxx == -1))
    {
        *calc_error = MULTIPLICATION_ERROR;
        goto MUL_END;
    }

    if(yyy > 0)
    {
        if((xxx > (INT64_MAX / yyy))||(xxx < (INT64_MIN / yyy)))
        {
        debug();
        *calc_error = MULTIPLICATION_ERROR;
        return result;
        }
    }else if(yyy < 0)
    {
    //y negative, so we check the abs value of y, but be weary if y is -1 
    //conveniently we can goto from here, so would just send the goto on return 0 with error code
        int64_t abs_yyy = abs(yyy);
        if((xxx > (INT64_MAX / abs_yyy))||(xxx < (INT64_MIN / abs_yyy)))
        {
        debug();
        *calc_error = MULTIPLICATION_ERROR;
        return result;
        }
    }

    result = (xxx * yyy);
    debug();

MUL_END:
    return result;


}

int64_t jet2holiday(){
    //print this if you detect bad input or over/underflow
    printf("\xe2\x99\xac\xe2\x99\xaa\xe2\x99\xb6\x20\x44\x61\x52\x6c\x31\x6e\x47\x20\x48\x6f\x4c\x64\x20\x4d\x79\x20\x48\x61\x41\x61\x41\x6e\x44\x21\x20\xe2\x99\xb6\xe2\x99\xaa\xe2\x99\xb6\x0a\x6e\x4f\x74\x48\x69\x4e\x67\x20\x42\x65\x41\x74\x53\x20\x61\x20\x4a\x65\x54\x32\x48\x6f\x6c\x49\x64\x41\x79\x20\x61\x4e\x64\x20\x52\x69\x67\x48\x74\x4e\x6f\x57\x20\x79\x4f\x75\x20\x43\x61\x4e\x20\x73\x41\x76\x45\x20\x5e\x20\x32\x0a\x35\x30\x4c\x62\x73\x20\x50\x65\x52\x70\x75\x72\x72\x53\x75\x6e\x20\x20\x54\x68\x41\x41\x41\x74\x27\x73\x20\x32\x6f\x30\x4c\x42\x73\x20\x6f\x46\x66\x66\x20\x34\x20\x40\x20\x46\x61\x4d\x69\x6c\x59\x20\x6f\x66\x20\x46\x6f\x57\x65\x72\x2e\x2e\x0a");
  return 1;
}
int64_t divide(int64_t xxx, int64_t yyy, int *calc_error){
    //div is a unique case
    int64_t result = 0;
    *calc_error = 0;
    debug();
    if(
        (-1 == xxx || -1 == yyy) && (INT64_MIN == yyy || INT64_MIN == xxx)
    )
    {
        *calc_error = DIVISION_ERROR;
    }else if (yyy == 0)
    {
        *calc_error = DIVISION_ZERO_ERROR;
    }else
    {
        result = (xxx / yyy);
    }
    return result;

}
int64_t modulo(int64_t xxx, int64_t yyy);

/* start of bitwise functions */

uint64_t shift_left(uint64_t number, uint64_t bits, int *calc_error){
    /* we'll have to do another separate check here so  that this one won't overflow since uint has one more bit highher thatn signed int*/
    uint64_t result = 0;
    //convert to double so pow will not cry
    double db_base = (double) 2;
    double db_bits = (double) bits;
    const int MAX_BITS = 31;

    if(bits > MAX_BITS)
    {
        *calc_error = SHIFT_ERROR;
        goto SHL_END;
    }

    uint64_t exponent = (uint64_t) pow(db_base, db_bits);
    uint64_t cieling = (UINT32_MAX / exponent);

    if(0 == bits){
        //no bits to move
        result = number;
    }
    else if(number > cieling)
    {
        *calc_error = SHIFT_ERROR;
    }
    else
    {
        result = number * exponent;
    }
SHL_END:
    //so this one works, you're just tired from 10<<31 lolb 
    debug();
    return result;
}

//BEGIN BITWISE
uint64_t bitwise_and(uint64_t first_num, uint64_t second_num, int *calc_error){
    /*input was checked before hand, no concern of over/underflow if input was within range*/
    debug();
    uint64_t result = 0;
    result = (first_num & second_num);
    return result;
}

uint64_t bitwise_or(uint64_t first_num, uint64_t second_num, int *calc_error){

    uint64_t result = 0;
    result = (first_num | second_num);
    return result;
}
uint64_t bitwise_exclusive_or(uint64_t first_num, uint64_t second_num, int *calc_error){

    uint64_t result = 0;
    result = (first_num ^ second_num);
    debug();
    return result;
}

uint64_t shift_right(uint64_t number, uint64_t bits, int *calc_error){
    
    /* we'll have to do another separate check here so  that this one won't overflow since uint has one more bit highher thatn signed int*/
    uint64_t result = 0;
    //convert to double so pow will not cry
    double db_base = (double) 2;
    double db_bits = (double) bits;
    const int MAX_BITS = 31;
    uint64_t exponent = 0;

    //you would not have the ceiling issue with this since it would always be diviging, as long as the bitJ
    if(0 == bits){
        //no bits to move
        result = number;
        debug();
    }else if(MAX_BITS < bits){
        *calc_error = SHIFT_ERROR;
    }else{
        uint64_t exponent = (uint64_t) pow(db_base, db_bits);
        result = number / exponent;
    }

    return result;
}
uint64_t rotate_left(uint64_t number, uint64_t bits, int *calc_error){
    /* rotate number by bits. stolen from geeksforgeeks.com*/
    //number = abs(number); optional
    //bits = abs(bits);
    //return (n << d) | (n >> (INT_BITS - d));
    *calc_error = 0;
    uint64_t return_me = 0;
    uint64_t num_shift_left = 0;
    uint64_t num_fall_off_bits = 0;
    uint64_t fall_shift_by = 0;

    if( (number < 0 || bits < 0) || (bits > MAX_INT_BITS) )  {
        //fix this damn if statement
        //keeps getting caught here for
        *calc_error = ROTATE_LEFT_ERROR;
        goto ROT_LEFT_EXIT;
    }
    else
    {
        //calculate should check if the uint value would be legal, so 
        //we do not have to worry
        uint64_t fall_shift_by = (MAX_INT_BITS - bits);
        uint64_t num_shift_left = shift_left(number, bits, calc_error);
        uint64_t num_fall_off_bits = shift_right(number, fall_shift_by, calc_error);

        //never hurts to double check (n >> (INT_BITS - d));
        if(*calc_error != 0){
            *calc_error = ROTATE_LEFT_ERROR;
            goto ROT_LEFT_EXIT;
        }
        //return me hits, 
        return_me = (num_shift_left | num_fall_off_bits );
    }

ROT_LEFT_EXIT:
    return return_me;
}
uint64_t rotate_right(uint64_t number, uint64_t bits, int *calc_error){
    *calc_error = 0;
    //apparently the larger the number, the less you need to rotate, 
    //copy pasted from rotate_left, just tweaked for ease,
    //can be refactored for (DRY principle)
    //number = abs(number); optional
    //bits = abs(bits);
    //return (n << d) | (n >> (INT_BITS - d));
    *calc_error = 0;
    uint64_t return_me = 0;
    uint64_t num_shift_left = 0;
    uint64_t num_fall_off_bits = 0;
    uint64_t fall_shift_by = 0;

    //FIX FUNCTIONS LIKE A SHIFT LEFT FOR NOW
    if( (number < 0 || bits < 0) || (bits > MAX_INT_BITS) )  
    {
        *calc_error = ROTATE_RIGHT_ERROR;
        goto ROT_RIGHT_EXIT;
    }
    else
    {
        uint64_t fall_shift_by = (MAX_INT_BITS - bits);
        //swapped the move right to the shift left heree
        uint64_t num_shift_left = shift_right(number, bits, calc_error);
        uint64_t num_fall_off_bits = shift_left(number, fall_shift_by, calc_error);
 
        if(*calc_error != 0)
        {
            *calc_error = ROTATE_RIGHT_ERROR;
            goto ROT_RIGHT_EXIT;
        }
        return_me = (num_shift_left | num_fall_off_bits );
    }

ROT_RIGHT_EXIT:
    return return_me;
}

void calculate(long xxx, char *operand, long yyy, int *ptr_calc_error, results *ptr_struct_result)
{
    //! change
    /**
     * @brief refactor operand to take in numbers not chars
     *        note:
     *        salvaged function from simp calc
     * 
     * @args: xxx: first_operand
     *        yyy: second_operands
     *        
     * @returns
              OUTPUT PARAMETER: results *ptr_struct_result
              //! implement
     *         error: -1
     *         success: 0
     */
    uint64_t u_first_number = 0;
    uint64_t u_second_number = 0;
    int64_t first_number = 0;
    int64_t second_number = 0;
    //int USE_UNSIGNED_INT = 1;
    int INVALID_UINT_VAL = uint_check_min_max(xxx, yyy);
    int INVALID_INT_VAL = int_check_min_max(xxx, yyy);
    
    char uint32_operands[7][3] = {"<<", ">>", "&","|", "^", "<<<", ">>>"};
    char int32_operands[5][2] = {"+", "-", "*", "/", "%"};

    //looks redundant but we could use this in the future as a boilerplate, maybe
    if(INVALID_UINT_VAL)
    {
        *ptr_calc_error = CHECK_UINT_MIN_MAX_ERROR;
    }
    else
    {
        u_first_number = xxx;
        u_second_number = yyy;
    }
    
    if(INVALID_INT_VAL)
    {
        *ptr_calc_error = CHECK_INT_MIN_MAX_ERROR;
    }
    else
    {
        first_number = xxx;
        second_number = yyy;
    }
    
    //implement a helper function ehre someday I guess.
    //the program states to just output lol we can print it here and //call it a day for now
    if(0 == (strcmp(operand, "<<") | INVALID_UINT_VAL) )
    {
        //stack gets here, but somehow, shl is 0?
        uint64_t result = shift_left(u_first_number, u_second_number, ptr_calc_error);

        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, ">>") | INVALID_UINT_VAL) )
    {
        uint64_t result = shift_right(u_first_number, u_second_number, ptr_calc_error);

        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
    }
    else if(0 == (strcmp(operand, "&") | INVALID_UINT_VAL) )
    {
        uint64_t result = bitwise_and(u_first_number, u_second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "|") | INVALID_UINT_VAL) )
    {
        uint64_t result = bitwise_or(u_first_number, u_second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "^") | INVALID_UINT_VAL) )
    {
        uint64_t result = bitwise_exclusive_or(u_first_number, u_second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
        debug();
    }
    //BEGIN SIGNED INT OPS
    else if (0 == (strcmp(operand, "+") | INVALID_INT_VAL) )
    {
        int64_t result = add(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_INT;
        ptr_struct_result->int32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "-") | INVALID_INT_VAL) )
    {
        int64_t result = subtract(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_INT;
        ptr_struct_result->int32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "*") | INVALID_INT_VAL) )
    {
        int64_t result = multiply(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_INT;
        ptr_struct_result->int32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "/") | INVALID_INT_VAL) )
    {
        int64_t result = divide(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_INT;
        ptr_struct_result->int32_result = result;
        debug();
    }
    //being unimplemented function
    else if (0 == (strcmp(operand, "<<<") | INVALID_UINT_VAL) )
    {
        uint64_t result = rotate_left(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        //caught bug here! 
        ptr_struct_result->uint32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, ">>>") | INVALID_UINT_VAL) )
    {
        uint64_t result = rotate_right(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        //caught bug here! //bug still here? idk, you took a fat break
        ptr_struct_result->uint32_result = result;
        debug();
    }


EXIT:
    return;
}
