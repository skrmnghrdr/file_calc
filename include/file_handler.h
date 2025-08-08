/**
 * This module is responsible for both file handling,
 * - header checking: checks if the file is valid, based on the specs
 * - header stamping: stamps the header appropriately 
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096U //probably a sweet spot since we used to do 4096 for dd


void file_handler();
int read_file();