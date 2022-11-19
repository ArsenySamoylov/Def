#ifndef DEBUG_FROM_NIGTH_TO_MORNIG
#define DEBUG_FROM_NIGTH_TO_MORNIG (false)

#include "my_buffer.h"

#define $i(int_var)     do { printf ("\t%s: %d\n", #int_var, int_var);        } while(0);  
#define $c(char_var)    do { printf ("\t%s: %c\n", #char_var, char_var);      } while(0);  
#define $d(double_var)  do { printf ("\t%s: %lg\n", #double_var, double_var); } while(0);  
#define $s(string)      do { printf ("\t%s: ", #string); ShowLine(string); printf("\n");      } while(0);
//#pragma GCC diagnostic ignored "-Wcast-qual"
#define $p(pointer)     do {printf("\t%s: %p\n", #pointer, (void*)pointer); } while(0);

#define assert(condition)   do                          \
                            {                           \
                            if (!(condition))           \
                                printf ("\033[91mASSERT\033[0m %s: Dyrachok, (%s) - is wrong condition (in line %d)\n", __func__, #condition, __LINE__); \
                            } while(0);

#define $                                                                                       \
    do                                                                                          \
    {                                                                                           \
        /*set_color*/                                                                           \
                                                                                                \
        printf("I AM WORKING IN FUNCTION: %s, in line: %d\n", __func__, __LINE__);/*add colors*/\
                                                                                                \
        /*reset_color*/                                                                         \
    } while (0);

//! @todo print all given parametrs (add later)
#define $$                                                          \
    do                                                              \
    {                                                               \
    printf("Enter any key to continue (line %d, func %s)\n", __LINE__, __func__);       \
    getchar();                                                      \
    } while (0);  
    
    
#endif