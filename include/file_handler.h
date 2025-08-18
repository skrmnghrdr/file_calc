/**
 * This module is responsible for both file handling,
 * - header checking: checks if the file is valid, based on the specs
 * - header stamping: stamps the header appropriately 
 * 
 */
#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <linux/limits.h> 
#include <sys/stat.h>
#include <sys/sysmacros.h>

//#include <hope.h>
#include <dirent.h>
#include "option_handler.h"
#include <sys/syscall.h>

#include "structs.h"
#include "operations.h"
//#include "operations.h"

#define BUFFER_SIZE 4096U //probably a sweet spot since we used to do 4096 for dd
#define MAX_RECURSION_DEPTH 8190 //$ ulimit -s - 2
#define HEADER_LENGTH 27 //27 bytes long per spec, see struct_file_header_t

//begin global vars

//end of struct definitions from specss




//!new functions revamped
int head_checker(char * abs_file_path);
int solve_directory(const char *input_dir, const char * output_dir);
int solve_file(int file_descriptor, const char* output_path);
const char *get_filename_ext(const char *filename);

//!old unused ones
void file_handler();
int read_file();
int check_file_header();
int print_header_byte_per_byte(struct struct_file_header_t file_header);
//https://stackoverflow.com/questions/16831605/strange-compiler-warning-c-warning-struct-declared-inside-parameter-list

#endif