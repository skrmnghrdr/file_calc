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

//#include <hope.h>
#include <dirent.h>
#include "option_handler.h"
#include <sys/syscall.h>

#define BUFFER_SIZE 4096U //probably a sweet spot since we used to do 4096 for dd
#define MAX_RECURSION_DEPTH 8190 //$ ulimit -s - 2

#define HEADER_LENGTH 27 //27 bytes long per spec, see struct_file_header_t

#pragma pack(1)
typedef struct struct_file_header_t 
{
    //as the specs dictates
    uint32_t magic_number;          //4 bytes long
    uint64_t file_id;               //8 bytes long
    uint64_t number_of_equations;   //8 bytes long
    uint8_t flag;                   //ONE byte only
    uint32_t equation_offset;       //4 bytes long
    uint16_t reserved;              //2 bytes long
} struct_file_header_t;
#pragma pack()

typedef struct input_dir_files 
{
    //all valid files (.equ) regardless of depth would be in **files
    //we might also just ommit them if they show signs of heresy on the header
    //looking at the spec, we could just put all .equ here and 
    //look at them at a deeper level
    char **files;
    int files_length;
} input_dir_files;

//https://linux.die.net/man/2/getdents64
//bro I'm so hungry T_T
struct linux_dirent64 {
    //struct straight from the docs
    uint64_t        d_ino;    //inode number, like auuid of some sort
    int64_t         d_off;  
    unsigned short  d_reclen; /*important, since this would be our offset
                                to the next entry in the dir*/
    unsigned char   d_type;   //see codes below
    char            d_name[]; 
    /*
    #define DT_UNKNOWN  0  // Unknown type
    #define DT_REG      8  // Regular file
    #define DT_DIR      4  // Directory
    #define DT_LNK     10  // Symbolic link
    #define DT_FIFO     1  // Named pipe (FIFO)
    #define DT_SOCK    12  // Socket
    #define DT_CHR      2  // Character device
    #define DT_BLK      6  // Block device

    */
};

int directory_crawler(struct options_t *opts);
void file_handler();
int read_file();
int check_file_header();
int print_header_byte_per_byte(struct struct_file_header_t file_header);
const char *get_filename_ext(const char *filename);
//https://stackoverflow.com/questions/16831605/strange-compiler-warning-c-warning-struct-declared-inside-parameter-list

#endif