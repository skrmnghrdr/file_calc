#ifdef DEBUG

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"

#define __FILENAME__ \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define PRINT_DEBUG(...)             \
    do                               \
    {                                \
        printf("%s@ %s%s:%s%d%s | ", \
               RED,                  \
               __FILENAME__,         \
               RESET,                \
               GREEN,                \
               __LINE__,             \
               RESET);               \
        printf(__VA_ARGS__);         \
    } while (0);

#else
#define PRINT_DEBUG(...)     \
    do                       \
    {                        \
        printf(__VA_ARGS__); \
    } while (0);

#endif 
// DEBUG



#include <stdio.h>
#include <stdlib.h>
#include "file_calculator.h"


int main(int argc, char ** argv)
{
    //!
    /* well have to czech the argv, but well
    have to put a const input for now. we will have to sanitize it here.
     * 
     * 
     *
     */
    //!
    file_calculator("../laboratory", "../output");
    return 0;
}
