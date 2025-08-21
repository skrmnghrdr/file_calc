/**
 * @file calculator.h //stolen from Mao's operations.h //simple calc
 * @author Mao, Smith zedonglayer@protonmail.com
 * @brief headerfile for operations.c
 * @version 0.1
 * @date 2025-07-22
 *
 *
 *
 */



#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_INT_BITS 32
#define MAX_OPERAND_STR_LEN 5
#define UINT32_MIN 0 
#define UINT64_MIN 0
//define struct flag
#define PRINT_UINT 1
#define PRINT_INT 2
//define error codes

//reduce to -1 and 0
#define CHECK_INT_MIN_MAX_ERROR 57
#define CHECK_UINT_MIN_MAX_ERROR 58
#define MULTIPLICATION_ZERO_ERROR 59
#define DIVISION_ZERO_ERROR 60
#define ADDITION_ERROR 61
#define SUBTRACTION_ERROR 62
#define MULTIPLICATION_ERROR 63
#define DIVISION_ERROR 64
#define SHIFT_ERROR 65
#define ROTATE_LEFT_ERROR 66
#define ROTATE_RIGHT_ERROR 67
#define MODULO_ERROR 68
#define INVALID_OPERATOR_ERROR 69
#define UNKNOWN_ERROR 999

extern int debug_on;


extern int64_t xxx;
extern int64_t yyy;

//debatable
void debug();
void usage();
int int_check_min_max(uint64_t xxx, uint64_t yyy);
int uint_check_min_max(uint64_t xxx, uint64_t yyy);

//STRUCT DECLARE
typedef struct results{
    int print_flag;
    int calc_error;
    int64_t int32_result;
    uint64_t uint32_result;
} results;

//main functions
void calculate(long xxx, char *operand, long yyy, int *ptr_calc_error,  results *ptr_struct_result);
int64_t add(int64_t xxx, int64_t yyy, int *calc_error);
int64_t subtract(int64_t xxx, int64_t yyy, int *calc_error);
int64_t multiply(int64_t xxx, int64_t yyy, int *calc_error);
int64_t jet2holiday();
int64_t divide(int64_t xxx, int64_t yyy, int *calc_error);
int64_t modulo(int64_t xxx, int64_t yyy);

//bitwise
uint64_t bitwise_and(uint64_t first_num, uint64_t second_num, int *calc_error);
uint64_t bitwise_or(uint64_t first_num, uint64_t second_num, int *calc_error);
uint64_t bitwise_exclusive_or(uint64_t first_num, uint64_t second_num, int *calc_error);
uint64_t shift_left(uint64_t number, uint64_t bits, int *calc_error);
uint64_t shift_right(uint64_t number, uint64_t bits, int *calc_error);
uint64_t rotate_left(uint64_t number, uint64_t bits, int *calc_error);
uint64_t rotate_right(uint64_t number, uint64_t bits, int *calc_error);