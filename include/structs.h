
#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdint.h>
#include <limits.h>

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
    //TOTAL                         //27BYTES TOTAL
} struct_file_header_t;

// unsolved_equation_t has dependency 
// with ser_equ_format_t
typedef struct ser_equ_format_t
{
    uint64_t    operand_first;      //8 bytes long
    uint8_t     operator;           //ONE byte only
    uint64_t    operand_second;     //8 bytes long
    //TOTAL                         //17 BYTES
} ser_equ_format_t;

typedef struct unsolved_equation_t
{
    uint32_t equation_id;            //4 bytes long
    uint8_t flags;                   //ONE byte only
    struct ser_equ_format_t ptr_equation; //17 fat bytes
    uint8_t padding[10];              //10 bytes pad

    //TOTAL                          //32 BYTES
} unsolved_equation_t;

typedef struct solved_equation_t
{
    uint32_t equation_id;            //4 bytes long
    uint8_t flags;                   //ONE byte only
    uint8_t type;                    //ONE byte only
    uint64_t solution;               //8 bytes long
} solved_equation_t;
#pragma pack()


//https://linux.die.net/man/2/getdents64
struct linux_dirent64 {
    //struct straight outta docs
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

typedef struct file_paths_t {
    char input_dir[PATH_MAX];
    char output_dir[PATH_MAX];
} file_paths_t;

#endif