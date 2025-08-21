#ifndef UTILS_H
#define UTILS_H
#ifdef DEBUG //! fix


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
#endif