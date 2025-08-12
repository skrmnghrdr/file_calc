
#include "file_handler.h"


/**
 * a temporary file handling program here, since we still are not sure 
 * how the file would be formatted, we know it would be little endian, but we
 * need to see the actual format and byte sequence on disk, that way we can load it.
 * it would be not good if we start writing it down right now just to tear it up 
 * 
 * UPDATE:
 * we found out that we can put an offset in memcopy, which would be safe too, but we would have to manually 
 * sloice and doice the buffer array, which would be easier to do.
 * with memcopy, we can tell it how many bytes and some pointer arithmetic as well.
 */
void file_handler(){

    printf("file handler called\n");
    return;
}

int read_file(){

    //we'll read from a static directory for now, just for the
    //experimental purposes
    FILE * ptr_file = fopen("../laboratory/aids", "rb");    
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    size_t total_bytes = 0;
    int return_me = -1;
    
    if(!ptr_file){
        perror("Error opening file\n");
        return 1;
    }

    do
    {
        bytes_read = fread(buffer, 1, BUFFER_SIZE, ptr_file);
        //fread returns the bytes_read
        if ( (bytes_read <= 0) && (ferror(ptr_file) != 0) )
        {
            //nothing was read, and error is not 0,
            goto READ_FILE_EXIT;
        }

        if (bytes_read > 0) //if there's bytes read
        {
            for(size_t curernt_read = 0; curernt_read < bytes_read; curernt_read++ )
            {
                //process the byte per byte here
                //you would need to read more than just a byte here
                //so we need to figure out a way to print and manipulate multi byte
                //also endianeessss
                printf("%02X ", buffer[curernt_read]);
            }
            total_bytes += bytes_read;
        }
    } while( (bytes_read <= BUFFER_SIZE) && (return_me == 0));
    printf("End of file reached\n");

READ_FILE_EXIT:
    return return_me;
}

int print_header_byte_per_byte(struct struct_file_header_t file_header)
{   
    //make something to print a file, and check if it's little or big endian
    //so something with 12345678 should print as 12 34 56 78 in byte per byte,
    //but if loaded with a uint32_t and printed, it would be 0x78563412 showing, 
    //then probably share it :)
    int bytes_len = sizeof(file_header);
    union union_file_header
    {
        // you can also put a struct here, that way
        //you can just print the whole damn thing
        struct_file_header_t header_struct; 
        uint8_t bytes[bytes_len]; //uint8_t is a byte, 32 is 4 long
    };
    //this is such magic,
    union union_file_header file_headerr;
    file_headerr.header_struct = file_header;

    
    for (int i = 0; i < bytes_len; i++)
    {
        printf("0x%X ", file_headerr.bytes[i]);
    }
    //magic right lol 
     printf("\n");

    return 0;
}
int check_file_header(){
    /*
    rework into read instead of fread

    */
    //**renovate this one  */
    FILE *file;
    struct_file_header_t file_header;

    file = fopen("../laboratory/aids", "rb");
    if (NULL == file)
    {
        perror("File empty or eerr\n");
        return -1;
    }
    unsigned char buffer[sizeof(file_header)];
    //we could put the whole max buffer here but it would just be safe to 
    //to just get the exact number of bytes
    size_t bytes_read = fread(buffer, 1, sizeof(file_header), file);
    if (bytes_read != sizeof(file_header))
    {
        fprintf(stderr, "Corrupted file!\n");
        fclose(file);
        return -1;
    }
    //when all is clear, time to copy the header tot eh file now
    memcpy(&file_header, buffer, sizeof(file_header));

    fclose(file);
    //printing something more than the byte would make the endianess
    //matter at this point, but iterating it byte per byte would 
    //not matter at all
    printf("Magic: 0x%X\n", file_header.magic_number);
    printf("File id: 0x%lX\n", file_header.file_id);
    printf("# of equations: 0x%lX\n", file_header.number_of_equations);
    
    print_header_byte_per_byte(file_header);
    return 0;

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

int directory_crawler(struct options_t *opts){
    /**
     * @brief this would be the function that crawls into the input 
     * directory and adds the files to struct input_dir_files
     * we then need to iterate through the input_dir_files.files and remove 
     * the invalid data or corrupted headers
     * 
     * @args 
     *       struct options_t *opts: pointer to options_t struct, change to just directory string someday for recursion
     *      char ***file_list: pointer to a string array. This is where we would append all the valid files 
     *      
     */
    int return_value = -1;
    unsigned char REGULAR_FILE = 8;
    unsigned char DIRECTORY = 4;
    const char *input_dir = opts->f_value;
    int target_directory = open(input_dir, O_RDONLY | __O_DIRECTORY);
    long getdents_bytes_read;
    struct linux_dirent64 *entity;
    char buf[BUFFER_SIZE]; 


    char **files; //realloc later if gets full since we have to call this recursiveley
    //or we could call this one outside the function, and pass it on a pointer here

    //this would check if the target is a dir or not
    if (-1 == target_directory){
        perror("open");
        return -1;
    }

    int nread;
    /*
        reading the docs slowly, I came across this one interesting piece.

        Note:  glibc  provides  no  wrapper for getdents(), necessitating the use of
        syscall(2).

        Note: There is no definition of struct linux_dirent in glibc; see NOTES.
    */
    //getdents64() implementation straight from the docs
    for (;;) 
    {
        getdents_bytes_read = syscall(SYS_getdents64, target_directory, buf, sizeof(buf));
        if (getdents_bytes_read == -1)
        {
            printf("error on syscal getdents64\n");
            goto END;
        }

        if (getdents_bytes_read == 0)
        {
            //verbose, print end of dir contents
            break;
        }

        for (size_t byte_ptr_offset = 0; byte_ptr_offset < getdents_bytes_read;)
        {
            entity = (struct linux_dirent64 *)(buf + byte_ptr_offset);
            //we need to recast the pointer since buffer is a character type,
            byte_ptr_offset += entity->d_reclen;
            //after dinner, we can now process the things here, if they're a dir, then store them in the buffer and get 
            //we would need to make a recursive function here, that would append 
            /**
             * gameplan would be
             * if dir, get base dir and call the function itself to crawl on that one, then we can append the files on a buffer outside this dir,
             * if the buffer is not big enough, we can always allocate more memory on the buffer inside the function here, using realloc.
             * we also set a counter to the max recursion limit, which would be the maximum avlue of an unsigned 32 bit number 
             * 
             */
            unsigned char valid_entity = (entity->d_type == DIRECTORY) || (entity->d_type == REGULAR_FILE);
            
            if(valid_entity)
            {
                //avoid anything else aside from files and dirs
                printf("Alright, we have a type:%d\n", entity->d_type);
                //check the filename here, then append to the file list
                const char *extension = get_filename_ext(entity->d_name);
                int invalid_extension = strcmp(extension, "equ");
                if(!invalid_extension)
                {
                    //do something here, append to file array
                    printf("VALID FILE:%s\n", entity->d_name);
                    //do something here, append to the string array with malloc
                    //that would increment and be resized as neeeded
                }
                
            }

        }

    }

    close(target_directory);
    return_value = 0;
END:
    return return_value;
}