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
#include "utils.h"
//#include "operations.h"
#define DEBUG //! DEBUG HERE

#define JANITOR(p) janitor((void**)&(p));


#define BUFFER_SIZE 4096U //probably a sweet spot since we used to do 4096 for dd
#define MAX_RECURSION_DEPTH 8190 //$ ulimit -s - 2
#define HEADER_LENGTH 27 //27 bytes long per spec, see struct_file_header_t

//begin global vars

//end of struct definitions from specss


/**
 * @brief sanitizes the pointers you give it :)
 *        (frees and assigns null to it)
 *        declare the macro below for cleaner usage:
 * 
 * 
 *        #define JANITOR(p) janitor((void**)&(p))
 * 
 *        then call it like so
 * 
 *        JANITOR(p_int_array);
 *   
 * 
 * 
 * @param ptr_dirty: the pointer to be cleared and
 *        nullified
 * 
 * @returns: VOID FUNCTION.
 * 
 */
void janitor(void **ptr_dirty);


//!new functions revamped
/**
//! WARNING: returns an open FILE DESCRIPTOR
//!         on success, and closes if failed.

* @brief:checks the permissions as stated on specs
*        checks the file header for legitimacy
* 
* @args:
*          abs_file_path: abs file path leading to the dir
*      
* 
* @returns:
*          success: 
*          int file_descriptor: fd of the opned file
*          failure:error:
*          int file_descriptor: -1 
*/
int head_checker(char * abs_file_path);

/**
* @brief stamps the VERIFIED file header from the input_fd
*        TO the output_fd
              //! this function does not check the header
              //! use head_checker() to verify input first.
* 
* @args: 
*       int input_fd: file desc where we copy the header from
*       int output_fd: file desc where we slap the header to
* 
* @return:
*        error: -1
*        success: 0
* 
*/
int header_slapper(int input_fd, int output_fd);

/**
* @brief: checks the directories enteties, and verifies the file ext,
*         verifies the header, feeds the file to file solver.
*          
* 
* @calls: file_checker()
*         append_path_and_file()
*         head_checker()
*         get_filename_ext()
*         solve_file()
* 
* @args: 
*          input dir: where we scan for .equ files
*          output dir: where to write the output
*          
* @returns: 
*          0 for no error
*          -1 for error
 */
int solve_directory(const char *input_dir, const char * output_dir);


/**
     * @brief: this function solves the file via process_equation() and calls file_write() 
*         to write it on output_file_desc. Use of fstat is recommended as we are   
*         working 
*         with file_desc
//! on failure, this function closes file_descriptor arg.
* 
 * @calls: file_write(), 
*         operations();
* 
 * @args: 
*       int file_descriptor: valid file_descriptor of the file 
*       we're solving
*       int output_file_desc: file desc to write the ouput
*/
int solve_file(int input_file_desc, int output_file_desc);


/**
 * @brief writes to the output file desc 
 * 
 * @param output_file_desc: a valid file desc for writing
 * @param solved_equ: the struct that will be written
 * @return int 
 *         err: -1
 *         succ: 0
 */
int write_output(int output_file_desc, solved_equation_t *solved_equ);

/**
* @brief https://stackoverflow.com/questions/5309471/getting-file-extension-in-c
* 
* args:
*      const char *filename: string filename to be processed
* return:
*      valid:
*      pointer to the first char after the period in the file extension
*      invalid:
*      returns nothing
*/
const char *get_filename_ext(const char *filename);

/**
 * @brief processes the valid file with a valid extension
 *        opens file desc if file was valid 
 * 
 * @param p_ent_buffer getdents64() return buffer
 * @param ent_buffer_size p_ent_buffer size
 * @param getdents64_bytes_read  
 * @return int 
 *         err: -1
 *         succ: 0
 */
int process_file(char *p_ent_buffer, int ent_buffer_size, long getdents64_bytes_read, struct file_paths_t file_paths);

/**
 * @brief: verifies the entities file_type & extension
          //! does not process the header, see header_checker()

 * @param entity: struct linux_dirent64 from
                  //https://linux.die.net/man/2/getdents64

 * @return int 
               err:-1
               succ:0
 */
int verify_entity(struct linux_dirent64 *entity);


//!old unused ones
void file_handler();
int read_file();
int check_file_header();
int print_header_byte_per_byte(struct struct_file_header_t file_header);
//https://stackoverflow.com/questions/16831605/strange-compiler-warning-c-warning-struct-declared-inside-parameter-list



#endif