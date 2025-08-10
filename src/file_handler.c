
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