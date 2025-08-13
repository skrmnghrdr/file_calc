/**
THE MAIN Daddy program for this entire software

 _____ ___  ____   ___  
|_   _/ _ \|  _ \ / _ \ 
  | || | | | | | | | | |
  | || |_| | |_| | |_| |
  |_| \___/|____/ \___/ 
  
  first logic: 
  logic would be file handler checks the file, if no error, 
  start parsing through it, then sends the parsed problenm to the operations section.
  operations section would reply with a solution, file handler writes it back,
  rinse wash repeat until it's done.

  update: 
  we now have the files, we have been playing with how the endianess works
  notice that it does not matter if we just use byte per byte, endianess 
  only matters when it is morethan two bytes,
  so to see the real picture, you can print it byte per byte, 
  and compare it when printing a multi byte, 

  eg:
  Magic: 0x38298199
  File id: 0x3928918312392819
  # of equations: 0x1238298193128312
  0x99 0x81 0x29 0x38 0x19 0x28 0x39 0x12 0x83 0x91 0x28 0x39 0x12 0x83 0x12 0x93 0x81 0x29 0x38 0x12 0x39 0x81 0x23 0x98 0x12 0x39 0x80 
  as you can see here, we are running on little endian, we might have to use a thrid party library to flip the magic number back to normal using endian.h

  done:
  get_option implemented

  next:
  fix the getoption and load the files in a buffer or somewhere to 
  have the os.path functionality here in C. 
  eat breakfast
  workout, push day, weighted <3

  post breakfast clarity:
  let's not use dirent.h since it does not have bonus points
  getdents() and getdents64() is where we'll go with

  we have to use read, instad of the soyboy fread() lmao


  CURRENT TASK:
  make the get_opt work with file handler 
    make the file handler crawl through the designated directory
    and log all the files (.equ)
    then check for the header, one by one
    

  we would need to implement the header checker, if 55 bb 77 dd, then good header, else corruputed header

  we need to find somewhat of a file crawler in C, to go through all the files in the directory

 */
#include <stdio.h>
#include <stdlib.h>
#include "file_calculator.h"


int main(int argc, char ** argv)
{
  /**
   * @brief Main for the file_calculator project
   * 
   */
  //char **file_list -> x  = ['hello', 'test']
  //char **file_list[] -> [['hello', 'this'], ['is','two', 'dimension']]
  unsigned int list_max_len = 2;
  unsigned int current_list_len = 0;
  char **file_list = calloc(list_max_len, sizeof(char *));//list of strings, 
  struct options_t opts = {0};
  int return_value = -1;

  if(file_list == NULL)
  {
    printf("Error allocating memory\n");
    goto END;
  }

  //mking the scaffolding, making sure I imported everything
  file_calculator();

  check_file_header(opts);

  process_options(argc, argv, &opts);
  //fix proc options 
  print_struct(&opts);

  directory_crawler(&opts, &file_list, &list_max_len, &current_list_len);
  for(int i = 0; i < current_list_len; i++)
  {
    printf("Content of file_list: %s\n", file_list[i]);
  }
END:
  return return_value;
}