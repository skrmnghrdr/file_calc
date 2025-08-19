
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
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;


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
END:
   return file_descriptor;
}


int solve_directory(const char *input_dir, const char * output_dir)
{
    /**
     * @brief: checks the directories enteties, and verifies the file ext,
     *          verifies the header, feeds the file to file solver
     * 
     * @calls: file_checker()
     *         append_path_and_file()
     *         head_checker()
     *         get_filename_ext()
     * 
     * @args: 
     *          input dir: where we scan for .equ files
     *          output dir: where to write the output
     *          
     * @returns: 
     *          0 for no error
     *          -1 for error
     */

    int return_value = -1;
    int input_directory = open(input_dir, O_RDONLY | O_DIRECTORY);
    //! learning point, you don't open the dir as a write,
    //! they're special files, you just rdolnly them
    int output_directory = open(output_dir, O_RDONLY | O_DIRECTORY); 
    unsigned char REGULAR_FILE = 8;
    unsigned char DIRECTORY = 4;
    struct stat st;

    //long for the 64 version
    long getdents_bytes_read;
    struct linux_dirent64 *entity;
    int nread;
    char *buf = malloc(BUFFER_SIZE);
    char *file_abs_path = malloc(PATH_MAX);
    char *output_abs_path = malloc(PATH_MAX); 
    /*
    wait, what if the file has write permissions on other places it should not have?
    I mean, whoever gave permissions on this file is a moron too then
    or whoever chmoded this file, so it's as good as any other priv esc binary at this point no?
    */

    if(!file_abs_path)
    {
        printf("! Malloc failed! input path\n");
        goto END;
    }
    
    if(!output_abs_path)
    {
        printf("! Malloc failed: output path");
        goto END;
    }

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
                printf("! Error on appending input file...\n");
                goto END_READ_DIR_ENT;
            }

            if (0 > output_pathname)
            {
                printf("! Error on appending ouput file...\n");
                goto END_READ_DIR_ENT;
            }

            //O_WRONLY | O_CREAT | O_TRUNC write and read, create if not there, overlap if exists
            int output_fd = open(output_abs_path, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
            if (0 > output_fd)
            {
                printf("! Error on creating/handling output file...\nSkipping\n");
                goto END_READ_DIR_ENT;
            }

            printf("valid file! %s\n", entity->d_name);
            printf("%s\n", file_abs_path);

            int valid_header = head_checker(file_abs_path);
            if (0 > valid_header)
            {
                printf("! Invalid header type!..\n");
                //skip
                goto END_READ_DIR_ENT;
            }

            //todo: function below 
            int was_unsolved = solve_file(valid_header, output_fd);
            if(was_unsolved)
            {
                printf("! Something wrong with file:%s skipping...\n", input_dir);
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
END:
    return return_value;
}


int solve_file(int input_file_desc, int output_file_desc)
{
    /**
     * @brief: this function solves the file and calls file_write() to write it on output_file_desc
     *         use of fstat is recommended as we are working with file_desc
              //! on failure, this function closes file_descriptor arg.
     * 
     * @calls: file_write(), operations();
     * 
     * @args: 
     *       int file_descriptor: valid file_descriptor of the file we're solving
     *       int output_file_desc: file desc to write the ouput
     */

    //!start yap
    /**
     * starts seeking via offset, starts loading a struct, (we might haev to make struct equation to) load it there.
     * calls the operation to solve it();
     * operation returns with the answer or something; () //utilize the  
     * 
     * check if the file exists, and if it does, fucking overwrite it anwyayas
     *  open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
     * print the O_CREATE FLAG to cerate a file, 
     * 
     * 
     * It is recommended to use an array to hold the data for each file, and dynamic memory
    allocation on the heap to hold all file data.
    I'm guessing we have to jsut dump everything in an array, since the file would not be that big of afile then
     */
    //! end yap

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
        printf("! Error on moving offset_to_equation lseek..\n");
        goto END;
    }

    for(uint32_t iter = 0; iter < file_header.number_of_equations; iter++)
    {
        //! kernel tracks the offset when we keep reading
        struct ser_equ_format_t serialized_equation = unsolved_equ.ptr_equation;

        //sanity check
        
        read(input_file_desc, &unsolved_equ, sizeof(unsolved_equation_t));
        printf("Values: equ id: %X\n", unsolved_equ.equation_id);
        printf("Operand_first: %ld, Operator %02x Operand_second: %ld\n",
                serialized_equation.operand_first,
                serialized_equation.operator,
                serialized_equation.operand_second
                );
        
        //! start yap
        //! limit functionality to basic functions for now
        
        /*
        we would pass the unsolved equ pointer and deal with it on operations function
        from there, we coudl deal with endianess,

        the output would always be solved_equation_t
        which the function would return and file_handler would just have to write it
        
        */
        //! so wouod the solved one only 
        //! end yap
        int process_equ_res = process_equation(&unsolved_equ, &solved_equ);
        if (0 > process_equ_res)
        {
            printf("! Something wrong went with processing the equation..\n");
            goto END;
        }
        //write output should just be the file descriptor of the 

        //! change this later
        int write_result = write_output(output_file_desc, &solved_equ);
        //! assuemdthe fiile is ready to be written
        //! header stamp
        //! content populate
        //printf("gdb anchor..\n");

    }

    return_value = 0;
END:
    return return_value;
}

int write_output(int output_file_desc, solved_equation_t *solved_equ)
{
    int return_me = -1; //:)
    if( NULL == solved_equ)
    {
        printf("! Solved equation pointer nulll...\n");
        goto END;
    }
    
    printf("About to write: ID: 0x%X\n", solved_equ->equation_id);

    write(output_file_desc, solved_equ, sizeof(*solved_equ));

END:
    return_me = 0;
    return return_me;
}

