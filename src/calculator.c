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
#include <calculator.h>
#include <math.h>
#include <string.h>



int debug_on = 0;
__INT32_TYPE__ x = 0;
__INT32_TYPE__ y = 0;

int calc_error = 0;
int *ptr_calc_error = &calc_error;



void debug(){
    /* debug purposes only, used to achor in gdb */
    return;
}

void usage(){
    printf("./simplecalc <argument1> <opearand> <argument2>\n");
    printf("example usage:\n./simplecalc 1 + 1\n");
    printf("./simplecalc 2 * 5\n");
    return;
}

int int_check_min_max(long xxx, long yyy){
    /**
     * @brief checks xxx and yyy if they're both legal values of int32_max
     * we initally put long since strtol already has error checkin if we provide a long-er value than long
     * we then do another checking inside the operand function
     * we will write a separate uint checker for our anxiety to 
     * calm down and not rely on the operand.
     */
    if(
        (xxx > INT32_MAX) || 
        (xxx < INT32_MIN) ||
        (yyy > INT32_MAX) ||
        (yyy < INT32_MIN)  )
    {
        return 1;
    }
    return 0;
}

int uint_check_min_max(long xxx, long yyy){
    /** @brief samesies with int, just uint */
    if(
        (xxx > UINT32_MAX) || 
        (xxx < UINT32_MIN) ||
        (yyy > UINT32_MAX) ||
        (yyy < UINT32_MIN)  )
    {
        return 1;
    }
    return 0;
}

int check_valid_input(__INT32_TYPE__ xxx, char operand, __INT32_TYPE__ yyy){
    //fix this later
    return 0;
}

__INT32_TYPE__ add(__INT32_TYPE__ xxx, __INT32_TYPE__ yyy, int *calc_error){
    /**
     * @brief adding two numbers with over and underflow checks
     * extern int calc_error returns error code
     */
    //check_int32(xxx, yyy); 
    //would work here as well, 
    //print the error
    //return exit code to user pass/fail
    __INT32_TYPE__ result = 0;
    *calc_error = 0;

    if(
        //overflow check
        ((xxx > 0) && (yyy > (INT32_MAX - xxx))) ||
        //underflow check
        ((xxx < 0 ) && (yyy < (INT32_MIN - xxx)))
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

__INT32_TYPE__ subtract(__INT32_TYPE__ xxx, __INT32_TYPE__ yyy, int *calc_error){
     __INT32_TYPE__ result = 0;
     *calc_error = 0;
    if(
        //overflow check
        ((xxx < 0) && (yyy > (INT32_MAX + xxx))) ||
        //underflow check
        ((xxx > 0 ) && (yyy < (INT32_MIN + xxx)))
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

__INT32_TYPE__ multiply(__INT32_TYPE__ xxx, __INT32_TYPE__ yyy, int *calc_error){

    __INT32_TYPE__ result = 0;
    *calc_error = 0;
    //do not forget to check for 0 since we are diving here
    
    //handle special bad cases here
    if( (0 == yyy || 0 == xxx )){
        goto MUL_END;
    }
    if ((xxx == INT32_MIN && yyy == -1) || (yyy == INT32_MIN && xxx == -1))
    {
        *calc_error = MULTIPLICATION_ERROR;
        goto MUL_END;
    }

    if(yyy > 0)
    {
        if((xxx > (INT32_MAX / yyy))||(xxx < (INT32_MIN / yyy)))
        {
        debug();
        *calc_error = MULTIPLICATION_ERROR;
        return result;
        }
    }else if(yyy < 0)
    {
    //y negative, so we check the abs value of y, but be weary if y is -1 
    //conveniently we can goto from here, so would just send the goto on return 0 with error code
        __INT32_TYPE__ abs_yyy = abs(yyy);
        if((xxx > (INT32_MAX / abs_yyy))||(xxx < (INT32_MIN / abs_yyy)))
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

__INT32_TYPE__ jet2holiday(){
    //print this if you detect bad input or over/underflow
    printf("\xe2\x99\xac\xe2\x99\xaa\xe2\x99\xb6\x20\x44\x61\x52\x6c\x31\x6e\x47\x20\x48\x6f\x4c\x64\x20\x4d\x79\x20\x48\x61\x41\x61\x41\x6e\x44\x21\x20\xe2\x99\xb6\xe2\x99\xaa\xe2\x99\xb6\x0a\x6e\x4f\x74\x48\x69\x4e\x67\x20\x42\x65\x41\x74\x53\x20\x61\x20\x4a\x65\x54\x32\x48\x6f\x6c\x49\x64\x41\x79\x20\x61\x4e\x64\x20\x52\x69\x67\x48\x74\x4e\x6f\x57\x20\x79\x4f\x75\x20\x43\x61\x4e\x20\x73\x41\x76\x45\x20\x5e\x20\x32\x0a\x35\x30\x4c\x62\x73\x20\x50\x65\x52\x70\x75\x72\x72\x53\x75\x6e\x20\x20\x54\x68\x41\x41\x41\x74\x27\x73\x20\x32\x6f\x30\x4c\x42\x73\x20\x6f\x46\x66\x66\x20\x34\x20\x40\x20\x46\x61\x4d\x69\x6c\x59\x20\x6f\x66\x20\x46\x6f\x57\x65\x72\x2e\x2e\x0a");
  return 1;
}
__INT32_TYPE__ divide(__INT32_TYPE__ xxx, __INT32_TYPE__ yyy, int *calc_error){
    //div is a unique case
    __INT32_TYPE__ result = 0;
    *calc_error = 0;
    debug();
    if(
        (-1 == xxx || -1 == yyy) && (INT32_MIN == yyy || INT32_MIN == xxx)
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
__INT32_TYPE__ modulo(__INT32_TYPE__ xxx, __INT32_TYPE__ yyy);

/* start of bitwise functions */

__UINT32_TYPE__ shift_left(__UINT32_TYPE__ number, __UINT32_TYPE__ bits, int *calc_error){
    /* we'll have to do another separate check here so  that this one won't overflow since uint has one more bit highher thatn signed int*/
    __UINT32_TYPE__ result = 0;
    //convert to double so pow will not cry
    double db_base = (double) 2;
    double db_bits = (double) bits;
    const int MAX_BITS = 31;

    if(bits > MAX_BITS)
    {
        *calc_error = SHIFT_ERROR;
        goto SHL_END;
    }

    __UINT32_TYPE__ exponent = (__UINT32_TYPE__) pow(db_base, db_bits);
    __UINT32_TYPE__ cieling = (UINT32_MAX / exponent);

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
__UINT32_TYPE__ bitwise_and(__UINT32_TYPE__ first_num, __UINT32_TYPE__ second_num, int *calc_error){
    /*input was checked before hand, no concern of over/underflow if input was within range*/
    debug();
    __UINT32_TYPE__ result = 0;
    result = (first_num & second_num);
    return result;
}

__UINT32_TYPE__ bitwise_or(__UINT32_TYPE__ first_num, __UINT32_TYPE__ second_num, int *calc_error){

    __UINT32_TYPE__ result = 0;
    result = (first_num | second_num);
    return result;
}
__UINT32_TYPE__ bitwise_exclusive_or(__UINT32_TYPE__ first_num, __UINT32_TYPE__ second_num, int *calc_error){

    __UINT32_TYPE__ result = 0;
    result = (first_num ^ second_num);
    debug();
    return result;
}

__UINT32_TYPE__ shift_right(__UINT32_TYPE__ number, __UINT32_TYPE__ bits, int *calc_error){
    
    /* we'll have to do another separate check here so  that this one won't overflow since uint has one more bit highher thatn signed int*/
    __UINT32_TYPE__ result = 0;
    //convert to double so pow will not cry
    double db_base = (double) 2;
    double db_bits = (double) bits;
    const int MAX_BITS = 31;
    __UINT32_TYPE__ exponent = 0;

    //you would not have the ceiling issue with this since it would always be diviging, as long as the bitJ
    if(0 == bits){
        //no bits to move
        result = number;
        debug();
    }else if(MAX_BITS < bits){
        *calc_error = SHIFT_ERROR;
    }else{
        __UINT32_TYPE__ exponent = (__UINT32_TYPE__) pow(db_base, db_bits);
        result = number / exponent;
    }

    return result;
}
__UINT32_TYPE__ rotate_left(__UINT32_TYPE__ number, __UINT32_TYPE__ bits, int *calc_error){
    /* rotate number by bits. stolen from geeksforgeeks.com*/
    //number = abs(number); optional
    //bits = abs(bits);
    //return (n << d) | (n >> (INT_BITS - d));
    *calc_error = 0;
    __UINT32_TYPE__ return_me = 0;
    __UINT32_TYPE__ num_shift_left = 0;
    __UINT32_TYPE__ num_fall_off_bits = 0;
    __UINT32_TYPE__ fall_shift_by = 0;

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
        __UINT32_TYPE__ fall_shift_by = (MAX_INT_BITS - bits);
        __UINT32_TYPE__ num_shift_left = shift_left(number, bits, calc_error);
        __UINT32_TYPE__ num_fall_off_bits = shift_right(number, fall_shift_by, calc_error);

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
__UINT32_TYPE__ rotate_right(__UINT32_TYPE__ number, __UINT32_TYPE__ bits, int *calc_error){
    *calc_error = 0;
    //apparently the larger the number, the less you need to rotate, 
    //copy pasted from rotate_left, just tweaked for ease,
    //can be refactored for (DRY principle)
    //number = abs(number); optional
    //bits = abs(bits);
    //return (n << d) | (n >> (INT_BITS - d));
    *calc_error = 0;
    __UINT32_TYPE__ return_me = 0;
    __UINT32_TYPE__ num_shift_left = 0;
    __UINT32_TYPE__ num_fall_off_bits = 0;
    __UINT32_TYPE__ fall_shift_by = 0;

    //FIX FUNCTIONS LIKE A SHIFT LEFT FOR NOW
    if( (number < 0 || bits < 0) || (bits > MAX_INT_BITS) )  
    {
        *calc_error = ROTATE_RIGHT_ERROR;
        goto ROT_RIGHT_EXIT;
    }
    else
    {
        __UINT32_TYPE__ fall_shift_by = (MAX_INT_BITS - bits);
        //swapped the move right to the shift left heree
        __UINT32_TYPE__ num_shift_left = shift_right(number, bits, calc_error);
        __UINT32_TYPE__ num_fall_off_bits = shift_left(number, fall_shift_by, calc_error);
 
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

void calculate(long xxx, char *operand, long yyy, int *ptr_calc_error, results *ptr_struct_result){
    /**
     * calculates based on the opeard and puts error on calc error as output parameter
     * where operand is a null terminated string
     * 
     * refractor this one to return an error code, not just on the structs itself
     * 
     * this should return an error code, so I was thinking, we need to check the operand first, before plugging in the xxx and yyy into a variable, that way we don't keep invalid numbers in the stack
     * 
     * with hoff's advice on the enum,
     * we could put it here first, wehere we check the operand, get the enum 
     * then we get the translation in our table
     * 
     * we then check if the int was valid or not (uint, int value)
     * then proceed to do the switch case
     * 
     * or we could iterate through the enum list, verifiy the value and just do the calculation since we already got it.
     * this would remove the struct from being used completely as well, but since we are refractoring it, idk, we might or might not keep it as well.
     */
    __UINT32_TYPE__ u_first_number = 0;
    __UINT32_TYPE__ u_second_number = 0;
    __INT32_TYPE__ first_number = 0;
    __INT32_TYPE__ second_number = 0;
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
        __UINT32_TYPE__ result = shift_left(u_first_number, u_second_number, ptr_calc_error);

        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, ">>") | INVALID_UINT_VAL) )
    {
        __UINT32_TYPE__ result = shift_right(u_first_number, u_second_number, ptr_calc_error);

        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
    }
    else if(0 == (strcmp(operand, "&") | INVALID_UINT_VAL) )
    {
        __UINT32_TYPE__ result = bitwise_and(u_first_number, u_second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "|") | INVALID_UINT_VAL) )
    {
        __UINT32_TYPE__ result = bitwise_or(u_first_number, u_second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "^") | INVALID_UINT_VAL) )
    {
        __UINT32_TYPE__ result = bitwise_exclusive_or(u_first_number, u_second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        ptr_struct_result->uint32_result = result;
        debug();
    }
    //BEGIN SIGNED INT OPS
    else if (0 == (strcmp(operand, "+") | INVALID_INT_VAL) )
    {
        __INT32_TYPE__ result = add(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_INT;
        ptr_struct_result->int32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "-") | INVALID_INT_VAL) )
    {
        __INT32_TYPE__ result = subtract(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_INT;
        ptr_struct_result->int32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "*") | INVALID_INT_VAL) )
    {
        __INT32_TYPE__ result = multiply(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_INT;
        ptr_struct_result->int32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, "/") | INVALID_INT_VAL) )
    {
        __INT32_TYPE__ result = divide(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_INT;
        ptr_struct_result->int32_result = result;
        debug();
    }
    //being unimplemented function
    else if (0 == (strcmp(operand, "<<<") | INVALID_UINT_VAL) )
    {
        __UINT32_TYPE__ result = rotate_left(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        //caught bug here! 
        ptr_struct_result->uint32_result = result;
        debug();
    }
    else if (0 == (strcmp(operand, ">>>") | INVALID_UINT_VAL) )
    {
        __UINT32_TYPE__ result = rotate_right(first_number, second_number, ptr_calc_error);
        
        ptr_struct_result->print_flag = PRINT_UINT;
        //caught bug here! //bug still here? idk, you took a fat break
        ptr_struct_result->uint32_result = result;
        debug();
    }

    
EXIT:
    return;
}
