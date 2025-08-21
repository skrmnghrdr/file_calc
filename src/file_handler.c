
#include "file_handler.h"

/**
 * ! test
 * ? test
 * TODO: test
 * @param test
 */

 //BEGIN GLOBAL VARIABLE
 //! ask where to declare this
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

    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename)
    {
        return "";
    }
    
    return dot + 1;
}


int head_checker(char * abs_file_path)
{
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
    struct file_paths_t file_paths;
    struct stat st;
    //long for the 64 version
    long getdents_bytes_read;
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

    for(                    ;                     ;                         ) //lol
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
        snprintf(file_paths.input_dir, PATH_MAX, "%s", input_dir);
        snprintf(file_paths.output_dir, PATH_MAX, "%s", output_dir);

        int process_file_err = process_file(buf, BUFFER_SIZE,  getdents_bytes_read, file_paths);
        if(-1 == process_file_err)
        {
            //PRINT_DEBUG("[] file_handler:solve_directory Error at process_file")
            goto END_SPIDER_LOOP;
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
        file_buffer = NULL;
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
        int write_result = write_output(output_file_desc, &solved_equ);
        //! header stamp to cehck if file should be solved or not
        //! content populate
END_FOR_LOOP:
    }
    return_value = 0;
END:
    free(file_buffer);
    return return_value;
}

int write_output(int output_file_desc, solved_equation_t *solved_equ)
{
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

int process_file(char *p_ent_buffer, int ent_buffer_size, long getdents64_bytes_read, struct file_paths_t file_paths ){
    //All reasonable effort shall be taken to keep the length of each function limited to no more than 100 lines. 70+ lmaooo
    //time to work out
    int output_fd;
    struct linux_dirent64 *entity; 
    int return_value = -1;
    int input_pathname;
    int output_pathname;
    int ent_validity;
    char *file_abs_path = malloc(PATH_MAX);
    memset(file_abs_path, 0, PATH_MAX);
    if(NULL == file_abs_path)
    {
        //PRINT_DEBUG("[] file_handler:process_file err; file_abs_path is NULL");
        goto END;
    }
    char *output_abs_path = malloc(PATH_MAX);
    memset(output_abs_path, 0, PATH_MAX);
    if(NULL == output_abs_path)
    {
        //PRINT_DEBUG("[] file_handler:process_file err; output_abs_path is NULL");
        goto END;
    }

    for( size_t byte_ptr_offset = 0; byte_ptr_offset < getdents64_bytes_read;){
        entity = (struct linux_dirent64 *)(p_ent_buffer + byte_ptr_offset);
        byte_ptr_offset += entity->d_reclen;

        ent_validity = verify_entity(*entity);
        if(-1 == ent_validity){
        //PRINT_DEBUG("[!] file_handler:process_file: invalid dir entitiy");
        goto SKIP_ENTITY;
        }
        input_pathname = append_path_and_file(file_paths.input_dir, entity->d_name, PATH_MAX, file_abs_path);
        output_pathname = append_path_and_file(file_paths.output_dir, entity->d_name, PATH_MAX, output_abs_path);

        if ((0 > input_pathname) || (0 > output_pathname)){
            //PRINT_DEBUG("[!]File Handler:solve_directory Error on appending input/ouput file...\n");
            goto END;
        }
        //O_WRONLY | O_CREAT | O_TRUNC write and read, create if not there, overlap if exists
        output_fd = open(output_abs_path, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
        if (0 > output_fd){
            printf("[!] File handler:sovle_directory: Error on creating/handling output file...\nSkipping\n");
            goto END;
        }

        int valid_header = head_checker(file_abs_path);
        if (0 > valid_header){
            printf("[!] File handler:sovle_directory: Invalid header type!..\n");
            //skip
            goto END;
        }

        int slap_result = header_slapper(valid_header, output_fd);
        if (0 > slap_result){
            printf("[!] File handler:sovle_directory: Something went wrong stamping the header.\n");
            //skip file
            goto END; 
        }
        //todo: function below 
        int was_unsolved = solve_file(valid_header, output_fd);
        if(was_unsolved){
            printf("[!] File handler:sovle_directory: Something wrong with file:%s skipping...\n", file_abs_path);
            goto END;
        }
            //! thou shall not forget
            //! close(output_fd);
            //! close(valid_file_descriptor);
SKIP_ENTITY:
        printf("\n");
        }
        return_value = 0;
END:
        return return_value;
}


int verify_entity(struct linux_dirent64 entity)
{
    unsigned char REGULAR_FILE = 8;
    unsigned char DIRECTORY = 4;
    int return_value = -1;
    
    unsigned char valid_entity = (entity.d_type == REGULAR_FILE);
    if(!valid_entity)
    {   
        //skip the file
        goto END;
    }
    const char *extension = get_filename_ext(entity.d_name);  
    int invalid_extension = strcmp(extension, "equ");
    if(invalid_extension)
    {
        //skip the file
        goto END;
    }
    return_value = 0;
END:
    return return_value;
}