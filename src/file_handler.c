
#include "file_handler.h"

/**
 * ! test
 * ? test
 * TODO: test
 * @param test
 */

 //BEGIN GLOBAL VARIABLE
 uint32_t LEGIT_HEADER = 0xDD77BB55;

static int append_path_and_file(const char *path, char *file, size_t buffer_size, char *buffer)
{
    /**
     * @brief: helper function to append the path and file name together
     *         char *buffer would be the output parameter
     *          
     * @args: 
     *         char *path: path string, parent path leading to file
     *         char *file: filename.
     *         char *buffer: output parameter to write on
     * 
     * @return:
     *          -1 for error
     *          0 success.
     *         output:
     *                 char *buffer: the aboslute path to the file    
     */
    size_t path_len = strlen(path);
    size_t filename_len = strlen(file);
    char last_character = path[path_len -1];
    int return_value = -1;

    if ( (path_len + filename_len) > (size_t) PATH_MAX )
    {
        printf("!Abs path overflows the linux max path! ..\n");
        goto END;
    }
    // / for linux
    int bytes_written;

    //! note: you cannot know how much size malloc allcoated,
    //! you need a separate counter for the size of the buffer.
    if( '/' == last_character)
    {
        bytes_written = snprintf(buffer, buffer_size, "%s%s", path, file);
    }
    else
    {
        bytes_written = snprintf(buffer, buffer_size, "%s/%s", path, file);
    }

    if (0 > bytes_written )
    {
        printf("!Nothing written on filename buffer, (append)\n");
        goto END;
    }

    if ( (path_len + filename_len) > (size_t) bytes_written)
    {
        printf("! Truncating happneed. path corrupted..\n");
        goto END;
    }
    else
    {
        return_value = 0;
    }

END:
    return return_value;
}
const char *get_filename_ext(const char *filename)
{
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
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename)
    {
        return "";
    }
    
    return dot + 1;
}


struct struct_file_header_t* file_checker(struct linux_dirent64* entity)
{
    //! might not be needed
    /**
     * @brief:  this function checks if the file has the right extension
     *          and the right header for processing
     * 
     * @args:   entity: struct linux_dirent64 from getdents64() man page
     * 
     * @returns:
     *          valid:
     *                  pointer to the file header for further process
     *          error:
     *                  returns a null pointer
     */
    struct struct_file_header_t *return_ptr = NULL;
    //!implement logic here
    return return_ptr;
}

int head_checker(char * abs_file_path)
{
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
    
    struct struct_file_header_t header_struct = {0};
    int file_descriptor = -1;
    char buffer[BUFFER_SIZE]; //! dead code?
    ssize_t bytes_read;
    loff_t lseek_return;


    //! check file permissions
    //! we use stat since we just have the file path, not yet opened
    /**
     * check file perms, then check header
     * 
     */
    file_descriptor = open(abs_file_path, O_RDONLY );

    if ( -1 == file_descriptor)
    {
        printf("! Error opening file!...\n");
        goto END;    
    }

    bytes_read = read(file_descriptor, &header_struct, sizeof(struct struct_file_header_t));
    if(
        (header_struct.magic_number != LEGIT_HEADER) ||
        (bytes_read != sizeof(struct_file_header_t))
      )
    {
        printf("! Corrupted header / invalid header\n");
        close(file_descriptor);
        file_descriptor = -1;
        goto END;
    }

    //lseek responsibly to the start 
    lseek_return = lseek64(file_descriptor, 0, SEEK_SET);
    if (-1 == lseek_return)
    {
        printf("! Error lseeking...\n");
        goto END;
    }

END:
   return file_descriptor;
}

int header_slapper(int input_fd, int output_fd)
{
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
    int return_me = -1;

    struct struct_file_header_t header_struct = {0};
    char *buffer;
    ssize_t bytes_read;
    ssize_t write_result;
    loff_t lseek_return;
    buffer = malloc(BUFFER_SIZE);

    if(NULL == buffer)
    {
        printf("! Mallocation failed :9...\n");
        goto END;
    }
    //read header
    bytes_read = read(input_fd, &header_struct, sizeof(header_struct));
    if( bytes_read < sizeof(header_struct))
    {
        printf("! Error reading header\n");
        goto END;
    }

    write_result = write(output_fd, &header_struct, sizeof(header_struct));
    if( write_result < sizeof(header_struct))
    {
        printf("! Header slapping corrupted..\n");
        goto END;
    }

    //open and lseek responsibly


    //write header

    return_me = 0;
END:
    free(buffer);
    return return_me;
}



int solve_directory(const char *input_dir, const char * output_dir)
{
    //! change params to p
    int return_value = -1;
    if ((NULL == input_dir) || (NULL == output_dir) )
    {
        printf("[!] file_handler:solve_directory(): print mesg fo null pointer here.\n");
        goto END;
    }
    //! check nulls
    int input_directory = open(input_dir, O_RDONLY | O_DIRECTORY);
    int output_directory = open(output_dir, O_RDONLY | O_DIRECTORY); 
    //! learning point, you don't open the dir as a write,
    //! they're special files, you just rdolnly them
    unsigned char REGULAR_FILE = 8;
    unsigned char DIRECTORY = 4;
    struct stat st;

    //long for the 64 version
    long getdents_bytes_read;
    struct linux_dirent64 *entity;
    int nread;
    //! clear buffer right after 
    char *buf = malloc(BUFFER_SIZE); 
    char *file_abs_path = malloc(PATH_MAX);

    if(!file_abs_path)
    {
        printf("! Malloc failed! input path\n");
        //! free the buffs here
        goto END;
    }

    char *output_abs_path = malloc(PATH_MAX); 
    if(!output_abs_path)
    {
        printf("! Malloc failed: output path");
        free(file_abs_path);
        file_abs_path = NULL;
        goto CLEAN_UP;
    }
    /*
    wait, what if the file has write permissions on other places it should not have?
    I mean, whoever gave permissions on this file is a moron too then
    or whoever chmoded this file, so it's as good as any other priv esc binary at this point no?
    */


    


    if ( !buf )
    {
        printf("! Malloc failed on buff! ...\n");
        goto END;
    }

    if ( (-1 == input_directory) || (-1 == output_directory))
    {
        printf("! Error on open: invalid target dir or output dir\n");
        goto END;
    }

    for(;;)
    {
        //todo: learning point. sizeof() gets the dtat type size, not the number of a #define
        getdents_bytes_read = syscall(SYS_getdents64, input_directory, buf, BUFFER_SIZE);

        if (-1 == getdents_bytes_read )
        {
            printf("! Error on getdents64()\n");
            goto END;
        }

        if ( 0 == getdents_bytes_read)
        {
            //finished reading dir
            break;
        }
        //! refractor to another function
        for( size_t byte_ptr_offset = 0; byte_ptr_offset < getdents_bytes_read;)
        {
            entity = (struct linux_dirent64 *)(buf + byte_ptr_offset);
            byte_ptr_offset += entity->d_reclen;
            
            unsigned char invalid_entity = (entity->d_type == REGULAR_FILE);
            if(!invalid_entity)
            {   
                //skip the file
                goto END_READ_DIR_ENT;
            }
            
            const char *extension = get_filename_ext(entity->d_name);

            int invalid_extension = strcmp(extension, "equ");
            if(invalid_extension)
            {
                //skip the file
                goto END_READ_DIR_ENT;
            }

            int input_pathname = append_path_and_file(input_dir, entity->d_name, PATH_MAX, file_abs_path);
            int output_pathname = append_path_and_file(output_dir, entity->d_name, PATH_MAX, output_abs_path);

            if (0 > input_pathname)
            {
                printf("[!]File Handler:solve_directory Error on appending input file...\n");
                goto END_READ_DIR_ENT;
            }

            if (0 > output_pathname)
            {
                printf("[!] File handler:sovle_directory: Err on appending ouput file...\n");
                goto END_READ_DIR_ENT;
            }

            //O_WRONLY | O_CREAT | O_TRUNC write and read, create if not there, overlap if exists
            int output_fd = open(output_abs_path, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
            if (0 > output_fd)
            {
                printf("[!] File handler:sovle_directory: Error on creating/handling output file...\nSkipping\n");
                goto END_READ_DIR_ENT;
            }

            printf("[/] valid file! %s\n", entity->d_name);
            printf("%s\n", file_abs_path);

            int valid_header = head_checker(file_abs_path);
            if (0 > valid_header)
            {
                printf("[!] File handler:sovle_directory: Invalid header type!..\n");
                //skip
                goto END_READ_DIR_ENT;
            }

            int slap_result = header_slapper(valid_header, output_fd);
            if (0 > slap_result)
            {
                printf("[!] File handler:sovle_directory: Something went wrong stamping the header.\n");
                //skip file
                goto END_READ_DIR_ENT; 
            }

            //todo: function below 
            int was_unsolved = solve_file(valid_header, output_fd);
            if(was_unsolved)
            {
                printf("[!] File handler:sovle_directory: Something wrong with file:%s skipping...\n", input_dir);
                goto END_READ_DIR_ENT;
            }
            //! thou shall not forget
            //! close(output_fd);
            //! close(valid_file_descriptor);
            
        printf("\n");
        END_READ_DIR_ENT:
        }
    END_SPIDER_LOOP:
    }

    //merica
    free(buf);
    free(file_abs_path);
    close(input_directory);
    close(output_directory);
    return_value = 0;

CLEAN_UP:
    //code cave
    /*
    if (!NULL == buf)
    {
        free(buf);
        buf == NULL;
    }
    */

END:
    return return_value;
}


int solve_file(int input_file_desc, int output_file_desc)
{
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

    /**
     * 
     * It is recommended to use an array to hold the data for each file,
     *  and dynamic memory allocation on the heap to hold all file data.
     * 
     for bigger files, make a buffer multiplied by unsolved equ format

     */

    struct stat stat_buffer;
    struct struct_file_header_t file_header;
    struct solved_equation_t solved_equ; //proc equ outoput;
    struct unsolved_equation_t unsolved_equ;
    int fstat_result;
    int return_value = -1;
    unsigned char *file_buffer;
    off_t lseek_return;
    ssize_t bytes_read;

    //! opening output file for writing
    //! dont' open it here
    fstat_result = fstat(input_file_desc, &stat_buffer);
    if(-1 == fstat_result)
    {
        close(input_file_desc);
        goto END;
    }
    printf("[*] New file!...\n\n");
    printf("File size: %jd bytes\n", (intmax_t) stat_buffer.st_size);
    file_buffer = malloc(stat_buffer.st_size + 1);
    if (NULL == file_buffer)
    {
        printf("! Malloc error for file buffer..\n");
        goto END;
    }

    //sanity check to print the bytes
    for(int i = 0; i < stat_buffer.st_size; i ++)
    {
        //printf("0x%02X ", (unsigned char) file_buffer[i]);
    }
    lseek_return = lseek64(input_file_desc, 0, SEEK_SET);
    if (-1 == lseek_return)
    {
        printf("! Error lseeki-ng file..\n");
        goto END;
    }

    bytes_read = read(input_file_desc, &file_header, sizeof(struct_file_header_t));

    //!  memcopy syntax if you need it
    //memcpy(&file_header, file_buffer, sizeof(file_header));

    printf("Magic Number:%X\noffset:%u equations #:%lu\n",
            file_header.magic_number, file_header.equation_offset, file_header.number_of_equations
            );

    //!offset would be from the start.
    loff_t offset_to_equation;
    offset_to_equation = lseek64(input_file_desc, file_header.equation_offset, SEEK_SET);
    if(0 > offset_to_equation)
    {
        printf("[!] File_handler:solve_file: Error on moving offset_to_equation lseek..\n");
        goto END;
    }

    //! header slap it here
    for(uint32_t iter = 0; iter < file_header.number_of_equations; iter++)
    {
        //! kernel tracks the offset when we keep reading
        struct ser_equ_format_t serialized_equation = unsolved_equ.ptr_equation;

        //sanity check
        //prints lil endianed
        read(input_file_desc, &unsolved_equ, sizeof(unsolved_equ));
        printf("Values: equ id: %X\n", unsolved_equ.equation_id);
        printf("Operand_first: %lX, Operator %02X Operand_second: %lX\n",
                serialized_equation.operand_first,
                serialized_equation.operator,
                serialized_equation.operand_second
                );
        
        //do not worry with endianess in memory, only when displaying it
        int process_equ_res = process_equation(&unsolved_equ, &solved_equ);
        if (0 > process_equ_res)
        {
            printf("[!] File_handler:solve_file: Something wrong went with processing the equation..\n");
            //! once done with file, mark header as not solved if error was ticked
            goto END_FOR_LOOP;
        }

        
        //printf("[&&] GDB Anchor for debugging\n\n");

        int write_result = write_output(output_file_desc, &solved_equ);
        //! header stamp to cehck if file should be solved or not
        //! content populate
END_FOR_LOOP:
    }
    return_value = 0;
END:
    return return_value;
}

int write_output(int output_file_desc, solved_equation_t *solved_equ)
{
    // somehiow compiler inlines this one lol as per gdb

    int return_me = -1; //:)
    ssize_t write_output; 

    if( NULL == solved_equ)
    {
        printf("! Solved equation pointer nulll...\n");
        goto END;
    }

    printf("About to write: ID: 0x%X\n\n", solved_equ->equation_id);
    write_output = write(output_file_desc, solved_equ, sizeof(*solved_equ));

    if(write_output < sizeof(*solved_equ))
    {
        printf("! Error occured on write..\n");
        printf("! Begin cleansing the file of heresey...\n");
        //! cleans or delete the file here someday in the future
        //we flee for now
        goto END;
    }
    //todo: slap the file header in the file output here later, then continue with the operations 


END:
    return_me = 0;
    return return_me;
}

