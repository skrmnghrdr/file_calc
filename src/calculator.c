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





int int_check_min_max(uint64_t xxx, uint64_t yyy){
    /**
     * @brief checks xxx and yyy if they're both legal values of int32_max
     * we initally put long since strtol already has error checkin if we provide a long-er value than long
     * we then do another checking inside the operand function
     * we will write a separate uint checker for our anxiety to 
     * calm down and not rely on the operand.
    
     */
    int return_me = 1;
    int x_var = (int64_t) xxx;
    int y_var = (int64_t) yyy;

    //! we do individual checks for easier debug
    if (x_var > INT64_MAX){
        PRINT_DEBUG("[!] Overflow, x greater than max..\n");
        goto END_CHECK;
    }
    if (x_var < INT64_MIN){
        PRINT_DEBUG("[!] Overflow, x less than min..\n");
        goto END_CHECK;
    }
    if (y_var > INT64_MAX){
        PRINT_DEBUG("[!] Overflow, y greater than max..\n");
        goto END_CHECK;
    }
    if (y_var < INT64_MIN){
        PRINT_DEBUG("[!] Overflow, y lesser than min..\n");
        goto END_CHECK;
    }
    return_me = 0;
END_CHECK:
    return return_me;
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

    PRINT_DEBUG("[*] Addition operator called\n");
    if(
        //overflow check
        ((xxx > 0) && (yyy > (INT64_MAX - xxx))) ||
        //underflow check
        ((xxx < 0 ) && (yyy < (INT64_MIN - xxx)))
    ){
        *calc_error = ADDITION_ERROR;
        return result;
    }else{   
        result = (xxx + yyy);
        return result;
    }
}

int64_t subtract(int64_t xxx, int64_t yyy, int *calc_error){
    int64_t result = 0;
    *calc_error = 0;

    PRINT_DEBUG("[*] Subtraction operator called\n");
    if(
        //overflow check
        ((xxx < 0) && (yyy > (INT64_MAX + xxx))) ||
        //underflow check
        ((xxx > 0 ) && (yyy < (INT64_MIN + xxx)))
    ){
        *calc_error = SUBTRACTION_ERROR;
        return result;
    }else{   
        result = (xxx - yyy);
        return result;
    }
}

int64_t multiply(int64_t xxx, int64_t yyy, int *calc_error){

    int64_t result = 0;
    *calc_error = 0;
    PRINT_DEBUG("[*] multiply operator called\n");
    if( (0 == yyy || 0 == xxx )){
        goto MUL_END;
    }
    if ((xxx == INT64_MIN && yyy == -1) || (yyy == INT64_MIN && xxx == -1)){
        *calc_error = MULTIPLICATION_ERROR;
        goto MUL_END;
    }

    if(yyy > 0){
        if((xxx > (INT64_MAX / yyy))||(xxx < (INT64_MIN / yyy))){
        *calc_error = MULTIPLICATION_ERROR;
        return result;
        }
    }else if(yyy < 0){
        int64_t abs_yyy = abs(yyy);
        if((xxx > (INT64_MAX / abs_yyy))||(xxx < (INT64_MIN / abs_yyy))){
        *calc_error = MULTIPLICATION_ERROR;
        return result;
        }
    }

    result = (xxx * yyy);

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

    PRINT_DEBUG("[*] divide operator called\n");
    if(
        (-1 == xxx || -1 == yyy) && (INT64_MIN == yyy || INT64_MIN == xxx)
    ){
        *calc_error = DIVISION_ERROR;
    }else if (yyy == 0){
        *calc_error = DIVISION_ZERO_ERROR;
    }else{
        result = (xxx / yyy);
    }
    return result;

}
int64_t modulo(int64_t value, int64_t divisor)
{
    PRINT_DEBUG("[*] modulo operator called\n");
    int64_t return_value = -1;
    if (0 == divisor){
        PRINT_DEBUG("[!!] Modulo error ");
        goto END;
    }

    return_value = (value % divisor);
    PRINT_DEBUG("[*] Modulo did not have an error\n");
END:
    return return_value;
}

/* start of bitwise functions */

uint64_t shift_left(uint64_t number, uint64_t bits, int *calc_error){
    /* we'll have to do another separate check here so  that this one won't overflow since uint has one more bit highher thatn signed int*/
    uint64_t result = 0;
    //convert to double so pow will not cry
    double db_base = (double) 2;
    double db_bits = (double) bits;
    const int MAX_BITS = 31;

    PRINT_DEBUG("[*]  Shift left called\n");
    if(bits > MAX_BITS){
        *calc_error = SHIFT_ERROR;
        goto SHL_END;
    }

    uint64_t exponent = (uint64_t) pow(db_base, db_bits);
    uint64_t cieling = (UINT32_MAX / exponent);

    if(0 == bits){
        result = number;
    }
    else if(number > cieling){
        *calc_error = SHIFT_ERROR;

    }else{
        result = number * exponent;
    }
SHL_END:
    //so this one works, you're just tired from 10<<31 
    return result;
}

//BEGIN BITWISE
uint64_t bitwise_and(uint64_t first_num, uint64_t second_num, int *calc_error){
    /*input was checked before hand, no concern of over/underflow if input was within range*/
    PRINT_DEBUG("[*] bitwise_and called\n");

    uint64_t result = 0;
    result = (first_num & second_num);
    return result;
}

uint64_t bitwise_or(uint64_t first_num, uint64_t second_num, int *calc_error){
    PRINT_DEBUG("[*] bitwise_and called\n");

    uint64_t result = 0;
    result = (first_num | second_num);
    return result;
}
uint64_t bitwise_exclusive_or(uint64_t first_num, uint64_t second_num, int *calc_error){
    PRINT_DEBUG("[*] bitwise_exclusive_or called\n");

    uint64_t result = 0;
    result = (first_num ^ second_num);
    return result;
}

uint64_t shift_right(uint64_t number, uint64_t bits, int *calc_error){
    PRINT_DEBUG("[*] shift_right called\n");

    /* we'll have to do another separate check here so  that this one won't overflow since uint has one more bit highher thatn signed int*/
    uint64_t result = 0;
    //convert to double so pow will not cry
    double db_base = (double) 2;
    double db_bits = (double) bits;
    const int MAX_BITS = 31;
    uint64_t exponent = 0;

    if(0 == bits){
        result = number;
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
    PRINT_DEBUG("[*] rotate_left called\n");

    *calc_error = 0;
    uint64_t return_me = 0;
    uint64_t num_shift_left = 0;
    uint64_t num_fall_off_bits = 0;
    uint64_t fall_shift_by = 0;

    //if(16 == bits){ //!leave for debug purposes
    //todo: figure out why the compuiler automatically uses 32bit on a small number

    if(  bits > MAX_INT_BITS )  {
        *calc_error = ROTATE_LEFT_ERROR;
        goto ROT_LEFT_EXIT;
    }
    
    fall_shift_by = (MAX_INT_BITS - bits);
    num_shift_left = ((uint64_t)number << bits);
    num_fall_off_bits = (number >> fall_shift_by);


    if(*calc_error != 0){
         *calc_error = ROTATE_LEFT_ERROR;
        goto ROT_LEFT_EXIT;
    }

    return_me = (num_shift_left | num_fall_off_bits );

    //}//! deebug end
ROT_LEFT_EXIT:
    return return_me;
}
uint64_t rotate_right(uint64_t number, uint64_t bits, int *calc_error){
    PRINT_DEBUG("[*] rotate_right called\n");

    *calc_error = 0;
    *calc_error = 0;
    uint64_t return_me = 0;
    uint64_t num_shift_left = 0;
    uint64_t num_fall_off_bits = 0;
    uint64_t fall_shift_by = 0;

    if( (number < 0 || bits < 0) || (bits > MAX_INT_BITS) )  {
        *calc_error = ROTATE_RIGHT_ERROR;
        goto ROT_RIGHT_EXIT;
    }

    fall_shift_by = (MAX_INT_BITS - bits);
    num_shift_left = (number >> bits);
    num_fall_off_bits = (number << fall_shift_by);
 
    if(*calc_error != 0){
        *calc_error = ROTATE_RIGHT_ERROR;
        goto ROT_RIGHT_EXIT;
    }

    return_me = (num_shift_left | num_fall_off_bits );

ROT_RIGHT_EXIT:
    return return_me;
}