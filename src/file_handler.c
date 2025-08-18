
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

    if ( !buf )
    {
        printf("! Malloc failed on buff! ...\n");
        goto END;
    }

    if ( (-1 == input_directory) || (-1 == output_directory))
    {
        printf("! Error on open: invalid target dir");
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

            //! no need to calculate since path_max is based on linux max path
            

            if(!file_abs_path)
            {
                printf("! Malloc failed!..\n");
                goto END;
            }

            append_path_and_file(input_dir, entity->d_name, PATH_MAX, file_abs_path);
            
            printf("valid file! %s\n", entity->d_name);
            printf("%s\n", file_abs_path);

            int valid_file_descriptor = head_checker(file_abs_path);
            if (0 > valid_file_descriptor)
            {
                printf("! Invalid header type!..\n");
                //skip
                goto END_READ_DIR_ENT;
            }
            //! we could save all the file desc in an array and isolate this one? but that might be too much veudeu.

            //todo: function below 
            int was_unsolved = solve_file(valid_file_descriptor, output_dir);
            if(was_unsolved)
            {
                printf("! Something wrong with file:%s skipping...\n", input_dir);
                goto END_READ_DIR_ENT;

            }

            //! further logic to be announced
            //call and process the damn function here
            /**
             * solve this shit()
             * 
             * if solve this shit is -1, then error happened, 
             * 
             //! this would be done then
             */
            
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


int solve_file(int file_descriptor, const char* output_path)
{
    /**
     * @brief: this function solves the file and calls file_write() to write it on output dir
     *         use of fstat is recommended as we are working with file_desc
              //! on failure, this function closes file_descriptor arg.
     * 
     * @calls: file_write(), operations();
     * 
     * @args: 
     *       int file_descriptor: valid file_descriptor of the file we're solving
     *       const char * output_path: the folder where we write the output
     *
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
    struct solved_equation_t solved_equ;
    struct unsolved_equation_t unsolved_equ;

    int fstat_result;
    int return_value = -1;
    unsigned char *file_buffer;
    off_t lseek_return;
    ssize_t bytes_read;

    fstat_result = fstat(file_descriptor, &stat_buffer);
    if(-1 == fstat_result)
    {
        close(file_descriptor);
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
    lseek_return = lseek64(file_descriptor, 0, SEEK_SET);
    if (-1 == lseek_return)
    {
        printf("! Error lseeki-ng file..\n");
        goto END;
    }

    bytes_read = read(file_descriptor, &file_header, sizeof(struct_file_header_t));

    //!  memcopy syntax if you need it
    //memcpy(&file_header, file_buffer, sizeof(file_header));

    printf("Magic Number:%X\noffset:%u equations #:%lu\n",
            file_header.magic_number, file_header.equation_offset, file_header.number_of_equations
            );

    //!offset would be from the start.
    loff_t offset_to_equation;
    offset_to_equation = lseek64(file_descriptor, file_header.equation_offset, SEEK_SET);
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
        
        read(file_descriptor, &unsolved_equ, sizeof(unsolved_equation_t));
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

        if (-1 == process_equ_res)
        {
            printf("! Something wrong went with processing the equation..\n");
            goto END;
        }
        */
        //printf("gdb anchor..\n");

    }



    
    return_value = 0;
END:
    return return_value;
}

