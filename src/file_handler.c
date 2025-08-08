
#include "file_handler.h"

#include <stdio.h>

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
                printf("%02X ", buffer[curernt_read]);
            }
            total_bytes += bytes_read;
        }
    } while( (bytes_read <= BUFFER_SIZE) && (return_me == 0));
    printf("End of file reached");

READ_FILE_EXIT:
    return return_me;
}