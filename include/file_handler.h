/**
 * This module is responsible for both file handling,
 * - header checking: checks if the file is valid, based on the specs
 * - header stamping: stamps the header appropriately 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 4096U //probably a sweet spot since we used to do 4096 for dd

#define HEADER_LENGTH 27 //27 bytes long per spec, see struct_file_header_t

#pragma pack(1)
typedef struct struct_file_header_t {
    uint32_t magic_number;          //4 bytes long
    uint64_t file_id;               //8 bytes long
    uint64_t number_of_equations;   //8 bytes long
    uint8_t flag;                   //ONE byte only
    uint32_t equation_offset;       //4 bytes long
    uint16_t reserved;              //2 bytes long
} struct_file_header_t;
#pragma pack()

void file_handler();
int read_file();
int check_file_header();
int print_header_byte_per_byte(struct struct_file_header_t file_header);