#define DEBUG
#include "file_handler.h"



 //BEGIN GLOBAL VARIABLE
 //! ask where to declare this
 uint32_t LEGIT_HEADER = 0xDD77BB55;

/**
 * @brief helper file to sanitize the input and load into the input/output file buffers
 * 
 * @param p_file_paths: the buffer to place input dir and ouputdir 
 * @param pinput_dir: input directory
 * @param poutput_dir output directory
 * @return int:
 *         on succ: 0
 *         on err: -1
 */
static int sanitize_file_paths(struct file_paths_t *p_file_paths, const char *pinput_dir, const char *poutput_dir)
{
    int return_me = -1;
    int input_to_buffer;
    int output_to_buffer;

    input_to_buffer = snprintf(p_file_paths->input_dir, PATH_MAX, "%s", pinput_dir);
    output_to_buffer = snprintf(p_file_paths->output_dir, PATH_MAX, "%s", poutput_dir);
    
    if(( input_to_buffer < 0) || (output_to_buffer < 0)){
        PRINT_DEBUG("[!!] Fatal error on loading paths to buffer");
        goto END;
    }
    if(( input_to_buffer >= PATH_MAX) || (output_to_buffer >= PATH_MAX)){
        PRINT_DEBUG("[!] Paths are too long, shorten it");
        goto END;
    }

    return_me = 0;
END:
    return return_me;
}

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
static int append_path_and_file(const char *path, char *file, size_t buffer_size, char *buffer)
{
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
    if( '/' == last_character){
        bytes_written = snprintf(buffer, buffer_size, "%s%s", path, file);
    }
    else{
        bytes_written = snprintf(buffer, buffer_size, "%s/%s", path, file);
    }

    if (0 > bytes_written ){
        printf("!Nothing written on filename buffer, (append)\n");
        goto END;
    }

    if ( (path_len + filename_len) > (size_t) bytes_written){
        printf("! Truncating happneed. path corrupted..\n");
        goto END;
    }
    else{
        return_value = 0;
    }

END:
    return return_value;
}

/**
 * @brief: helper function to close, opened file descriptors:
 *         CHECKS the file_desc if valid before closing it
 * 
 * @args: file_descriptor:
 * 
 * @returns: VOID 
 *            
 */
static void fd_closer(int file_descriptor)
{
    if(0 > file_descriptor){
        //was not opened successfully, no need to close
        goto END;
    }
    close(file_descriptor);

END:
    return;
}


void janitor(void **ptr_dirty)
{
    free(*ptr_dirty);
    *ptr_dirty = NULL;
}

const char *get_filename_ext(const char *filename)
{

    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename){
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

    if(NULL == buffer){
        printf("! Mallocation failed :9...\n");
        goto END;
    }
    //read header
    bytes_read = read(input_fd, &header_struct, sizeof(header_struct));
    if( bytes_read < sizeof(header_struct)){
        printf("! Error reading header\n");
        goto END;
    }

    write_result = write(output_fd, &header_struct, sizeof(header_struct));
    if( write_result < sizeof(header_struct)){
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



int solve_directory(const char *pinput_dir, const char *poutput_dir)
{
    //! change params to p
    int return_value = -1;
    if ((NULL == pinput_dir) || (NULL == poutput_dir) ){
        PRINT_DEBUG("[!!] file_handler:solve_directory(): print mesg fo null pointer here.\n");
        goto END;
    }
    int input_directory = open(pinput_dir, O_RDONLY | O_DIRECTORY);
    int output_directory = open(poutput_dir, O_RDONLY | O_DIRECTORY); 
    struct file_paths_t file_paths;
    int path_sanitized;
    //long for the 64 version
    long getdents_bytes_read;
    
    //! clear buffer right after 
    char *buf = malloc(BUFFER_SIZE); 
    char *file_abs_path = malloc(PATH_MAX);
    char *output_abs_path = malloc(PATH_MAX); 
    if(!file_abs_path || !output_abs_path || !buf){
        PRINT_DEBUG("[!!] Malloc failed! on buffers path\n");
        goto CLEAN_UP;
    }

    if ( (-1 == input_directory) || (-1 == output_directory)){
        PRINT_DEBUG("[!!] Error on open: invalid target dir or output dir\n");
        goto CLEAN_UP;
    }
    
    path_sanitized = sanitize_file_paths(&file_paths, pinput_dir, poutput_dir);
    if (-1 == path_sanitized){
        PRINT_DEBUG("[!!] Filepath unclean, halting program..");
        goto CLEAN_UP;
    }

    for(                    ;                     ;                         ){ //!lol

        getdents_bytes_read = syscall(SYS_getdents64, input_directory, buf, BUFFER_SIZE);
        if (-1 == getdents_bytes_read ){
            PRINT_DEBUG("[!] Error on getdents64()");
            goto CLEAN_UP;
        }

        if ( 0 == getdents_bytes_read){
            PRINT_DEBUG("[/] Finished reading dir..\n");
            break;
        }

        int process_file_err = process_file(buf, BUFFER_SIZE,  getdents_bytes_read, file_paths);
        if(-1 == process_file_err){
            PRINT_DEBUG("[!] file_handler:solve_directory Error at process_file");
            goto END_SPIDER_LOOP;
        }

    END_SPIDER_LOOP:
    }

    return_value = 0;
CLEAN_UP:
    PRINT_DEBUG("[*] Cleaning up buffers");
    JANITOR(buf);
    JANITOR(file_abs_path);
    JANITOR(output_abs_path);
    fd_closer(input_directory);
    fd_closer(output_directory);

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
    if(-1 == fstat_result){
        close(input_file_desc);
        goto END;
    }
    PRINT_DEBUG("[*] New file!...\n\n");
    PRINT_DEBUG("File size: %jd bytes\n", (intmax_t) stat_buffer.st_size);
    file_buffer = malloc(stat_buffer.st_size + 1);
    if (NULL == file_buffer){
        PRINT_DEBUG("! Malloc error for file buffer..\n");
        goto END;
    }

    lseek_return = lseek64(input_file_desc, 0, SEEK_SET);
    if (-1 == lseek_return){
        printf("! Error lseeki-ng file..\n");
        goto END;
    }

    bytes_read = read(input_file_desc, &file_header, sizeof(struct_file_header_t));

    //!  memcopy syntax if you need it
    //memcpy(&file_header, file_buffer, sizeof(file_header));
    PRINT_DEBUG("Magic Number:%X\noffset:%u equations #:%lu\n",
            file_header.magic_number, file_header.equation_offset, file_header.number_of_equations
            );

    //!offset would be from the start.
    loff_t offset_to_equation;
    offset_to_equation = lseek64(input_file_desc, file_header.equation_offset, SEEK_SET);
    if(0 > offset_to_equation){
        PRINT_DEBUG("[!] File_handler:solve_file: Error on moving offset_to_equation lseek..\n");
        goto END;
    }

    //! header slap it here
    for(uint32_t iter = 0; iter < file_header.number_of_equations; iter++){
        struct ser_equ_format_t serialized_equation = unsolved_equ.ptr_equation;
        //prints lil endianed
        read(input_file_desc, &unsolved_equ, sizeof(unsolved_equ));
        PRINT_DEBUG("Values: equ id: %X\n", unsolved_equ.equation_id);
        PRINT_DEBUG("Operand_first: %lX, Operator %02X Operand_second: %lX\n",
                serialized_equation.operand_first,
                serialized_equation.operator,
                serialized_equation.operand_second);
        
        //do not worry with endianess in memory, only when displaying it
        int process_equ_res = process_equation(&unsolved_equ, &solved_equ);
        if (0 > process_equ_res){ 
            PRINT_DEBUG("[!] File_handler:solve_file: Something wrong went with processing the equation..\n");
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
    JANITOR(file_buffer);
    return return_value;
}

int write_output(int output_file_desc, solved_equation_t *solved_equ)
{
    int return_me = -1; //:)
    //int res_invalidate_header;
    ssize_t write_output; 

    if( NULL == solved_equ){
        PRINT_DEBUG("! Solved equation pointer nulll...\n");
        goto END;
    }

    PRINT_DEBUG("About to write: ID: 0x%X\n\n", solved_equ->equation_id);
    /**
     @todo do this, to make it spec compliant for next week.
     * 
     * since this function is responsible for writing in the file, we could check if
     * the equation flag has been solved or 0 if not, if 0, we call the invalidate header function
     * which just takes in the outputfile desc, and changes the header
     * 
     */
    /*psuedo code
    if(!solved_equ->flags){
        //error on solving, then invalidate header
        res_invalidate_header = invalidate_header(output_file_desc)
    }
    if(0 > res_invalidate_header){
        PRINT_DEBUG("[!!] Error on invalidating the header..\n");
        goto END;
    }
    */
    write_output = write(output_file_desc, solved_equ, sizeof(*solved_equ));

    if(write_output < sizeof(*solved_equ)){
        //failing to write it here makes the file corrupt
        //do something with it in the future
        PRINT_DEBUG("[!] Error occured on write..\n");
        PRINT_DEBUG("[!] Begin cleansing the file of heresey...\n");
        //! cleans or delete the file here someday in the future
        //we flee for now
        goto END;
    }
    //todo: slap the file header in the file output here later, then continue with the operations 

    return_me = 0;
END:
    return return_me;
}

int process_file(char *p_ent_buffer, int ent_buffer_size, long getdents64_bytes_read, struct file_paths_t file_paths ){
    //All reasonable effort shall be taken to keep the length of each function limited to no more than 100 lines. 70+ lmaooo
    int output_fd;
    struct linux_dirent64 *entity; 
    int return_value = -1;
    int input_pathname;
    int output_pathname;
    int ent_validity;
    char *file_abs_path = calloc(PATH_MAX, sizeof(char));
    char *output_abs_path = calloc(PATH_MAX, sizeof(char));

    if((!file_abs_path)|| (!output_abs_path)){
        PRINT_DEBUG("[!!] path buffer failed to allocate!.");
        goto END;
    }

    for( size_t byte_ptr_offset = 0; byte_ptr_offset < getdents64_bytes_read;){
        entity = (struct linux_dirent64 *)(p_ent_buffer + byte_ptr_offset);
        byte_ptr_offset += entity->d_reclen;

        ent_validity = verify_entity(entity);
        if(-1 == ent_validity){
            PRINT_DEBUG("[!] file_handler:process_file: invalid dir entitiy, skipping..");
            goto SKIP_ENTITY;
        }

        input_pathname = append_path_and_file(file_paths.input_dir, entity->d_name, PATH_MAX, file_abs_path);
        output_pathname = append_path_and_file(file_paths.output_dir, entity->d_name, PATH_MAX, output_abs_path);

        if ((0 > input_pathname) || (0 > output_pathname)){
            PRINT_DEBUG("[!]File Handler:solve_directory Error on appending input/ouput file...\n");
            goto END;
        }
 
        //O_WRONLY | O_CREAT | O_TRUNC write and read, create if not there, overlap if exists
        output_fd = open(output_abs_path, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
        if (0 > output_fd){
            PRINT_DEBUG("[!] File handler:sovle_directory: Error on creating/handling output file...\nSkipping\n");
            goto END;
        }

        int valid_header = head_checker(file_abs_path);
        if (0 > valid_header){
            PRINT_DEBUG("[!] File handler:sovle_directory: Invalid header type!..\n");
            goto END;
        }

        int slap_result = header_slapper(valid_header, output_fd);
        if (0 > slap_result){
            PRINT_DEBUG("[!] File handler:sovle_directory: Something went wrong stamping the header.\n");
            goto END; 
        }
        
        int was_unsolved = solve_file(valid_header, output_fd);
        if(was_unsolved){
            PRINT_DEBUG("[!] File handler:sovle_directory: Something wrong with file:%s skipping...\n", file_abs_path);
            goto END;
        }

SKIP_ENTITY:
        printf("\n");
        }//end dir ent for loop
        return_value = 0;

END:
        JANITOR(file_abs_path);
        JANITOR(output_abs_path);
        return return_value;
}


int verify_entity(struct linux_dirent64 *entity)
{
    unsigned char REGULAR_FILE = 8;
    unsigned char DIRECTORY = 4;
    int return_value = -1;
    
    unsigned char valid_entity = (entity->d_type == REGULAR_FILE);
    if(!valid_entity){   
        PRINT_DEBUG("[!] Invalid entitity %s, skipping..\n", entity->d_name);
        goto END;
    }
    const char *extension = get_filename_ext(entity->d_name);  
    int invalid_extension = strcmp(extension, "equ");
    if(invalid_extension){
        PRINT_DEBUG("[!] Wrong extension, skipping...");
        goto END;
    }
    return_value = 0;
END:
    return return_value;
}