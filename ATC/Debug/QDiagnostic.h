#pragma once

#include <errno.h>

#pragma GCC diagnostic ignored "-Wunused-value"
//! @note Use this macros with return val (for example: CHECK_PTR_RET(ptr, return NULL_PTR)) 



/*                                                                        
#define status(expected_status, ...) int status = __VA__ARGS__;
                    if (status != expected_status)
*/

/*
#define CHECK_PTR(ptr, return_expression)                                       \
                                          {                                     \
                                          if(!ptr)                              \
                                            {                                   \
                                            printf("\033[53m" "%s error: %s - NULL ptr (in line %d)\033[0m\n", __func__, __LINE__, #ptr);    \
                                            return_expression;                  \
                                            }                                   \
                                          } 
*/

// const int MAX_MESSAGE_LENGTH_FOR_STDERRR = 300;

/*
#define CHECK_STDERR(ptr, return_expression)                                    \
                                          {                                     \
                                          if(!ptr)                              \
                                            {                                   \
                                            char      message [MAX_MESSAGE_LENGTH_FOR_STDERRR + 16]  = {}; \
                                            snprintf (message, MAX_MESSAGE_LENGTH_FOR_STDERRR, "%s error: %s - NULL ptr (in line %d)", __func__, #ptr, __LINE__); \
                                            perror   (message);                 \
                                            return_expression;                  \
                                            }                                   \
                                          }       
                                                                                      
*/
