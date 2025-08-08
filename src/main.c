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

 */
#include <stdio.h>
#include <stdlib.h>
#include "file_calculator.h"


 int main(int argc, char ** argv){
   //mking the scaffolding, making sure I imported everything
   file_calculator();
   read_file();
    return 0;
 }