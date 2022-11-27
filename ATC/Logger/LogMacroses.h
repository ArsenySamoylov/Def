#pragma once

#include <iostream>
#include <errno.h>
#include <string.h>

#include "Logger.h"
#include "FunctionLogger.h"

#include "CommonEnums.h"
#include "LinuxColors.h"
#include "Utils.h"

#define LOG__ Logger::getInstance()

#define log_function        LOG__.log("%s: ", __func__)
#define LOG log_function;   LOG__ 

// .log("") to set indent
#define flog LOG__.log(""); LOG__ 
#define logf(format, ...)   LOG__.log(format __VA_OPT__(,) __VA_ARGS__)
#define logf_ni(format, ...)   LOG__.log_no_indent(format __VA_OPT__(,) __VA_ARGS__)

#define CALLOC(number_of_elements, size_of_element)  LOG__.CAL_LOG (number_of_elements, size_of_element, __FILE__, __func__, __LINE__);
#define KILL(ptr)                                    LOG__.FREE_LOG(ptr, __FILE__, __func__, __LINE__);
#define PASTE(x, y) x ## _ ## y
#define EVAL(x,y) PASTE(x,y)
#define NAME(func) EVAL(func, __LINE__)

#define $log(level) FunctionLogger NAME(func) {level, __func__};

#define LSUCCESS LOG__.LogMsgRet (SUCCESS, "%s:%d returned SUCCESS ", __func__, __LINE__)
#define LFAILURE LOG__.LogMsgRet (MsgRet(FAILURE, redcolor "Failed (%s:%d)" resetconsole,  __func__, __LINE__), "Failed (%s:%d)",  __func__, __LINE__)

#define LNULL    LOG__.LogMsgNullRet(LogMsgNullRet(redcolor "Returnig null ptr (%s:%d)" resetconsole, __func__, __LINE__), "Returnig null ptr (%s:%d)", __func__, __LINE__)

#define LogMsgRet(ret_val, format, ...) LOG__.LogMsgRet (ret_val, format __VA_OPT__(,) __VA_ARGS__)

#define CHECK(condition, what_to_do_if_false, ...)                                        \
                                              do                                          \
                                              {                                           \
                                              if (!condition)                             \
                                                {                                         \
                                                logf("%s:%d::CHECK: " #condition " is false\n", __func__, __LINE__);  \
                                                what_to_do_if_false ;                     \
                                                __VA_ARGS__ __VA_OPT__(;)                 \
                                                }                                         \
                                              }while (0);

#define assertlog(condition, error_code, return_expression) do                                                           \
                            {                                                                                            \
                            if (!(condition))                                                                            \
                                {                                                                                        \
                                LOG__.log("Condition (%s) is false\n", #condition);                                      \
                                printf("Condition" redcolor " (%s) is false\n" resetconsole, #condition);                \
                                LOG__.log("\t(function: %s, file: %s, line: %d)\n",                                      \
                                           __func__,   __FILE__, __LINE__);                                              \
                                printf("\t(function: %s, file: %s, line: %d)\n",                                         \
                                           __func__,   __FILE__, __LINE__);                                              \
                                LOG__.log("It matches to error: (code %d) %s\n\n", error_code, strerror(error_code));    \
                                LOG__.log("Shutting down the system (%s:%d)", __func__, __LINE__);                       \
                                           system("powerof");                                                            \
                                return_expression;                                                                       \
                                }                                                                                        \
                                                                                                                         \
                            } while (0);


#define $li(int_var)     do { logf ("\t%s: %d\n", #int_var, int_var);        } while(0);  
#define $lc(char_var)    do { logf ("\t%s: '%c'\n", #char_var, char_var);    } while(0);  
#define $ld(double_var)  do { logf ("\t%s: %lg\n", #double_var, double_var); } while(0);  
#define $ls(string)      do { logf ("\t%s: \"%s\"\n", #string, string);      } while(0);
#define $lp(pointer)     do { logf ("\t%s: %p\n", #pointer, (void*)pointer); } while(0);


#define TODO(message) printf(redcolor "#TODO Check this place to improve it (%s::%d)\n%s" resetconsole , __func__, __LINE__, message);      \
                                 logf("#TODO Check this place to improve it (%s%d)\n", __func__, __LINE__);                                 \
                                 logf(message);

#define YOU_SHALL_NOT_PASS printf(purplecolor "You shouldn't be able to reach this place (%s::%d)\n" resetconsole , __func__, __LINE__);     \
                                         logf("You shouldn't be able to reach this place (%s::%d)\n", __func__, __LINE__);

// IDEAS: logif (condition, smt to log) (basiclly its assertlog)
//! @note if you want to change log file name
//!          than define LOGNAME with name before including this file
//! @note if you want to show logs in console
//!          than define LOGNAME without name

/*
//! @note DetailedLog includes function name and location (line, file) 
#define dlog(format, ...) do                                                                                      \
                            {                                                                                     \
                            printf (my_log, "from %s (in line %d, file %s):\n\t", __func__, __LINE__, __FILE__);  \
                            printf (my_lof, format, __VA_ARGS__);                                                 \
                            } while (0);
*/

/* // If you want to return, abort or exit use ...
#define logif(condition, error_code, ...) do                                                                       \
                            {                                                                                      \
                            if (!condition)                                                                        \
                                {                                                                                  \
                                log("Condition (%s) is false\n", #condition);                                      \
                                log("\t(function: %s, file: %s, line: %d)\n",                                      \
                                           __func__,   __FILE__, __LINE__);                                        \
                                                                                                                   \
                                log("It matches to error: (code %d) %s\n\n", error_code, strerror(error_code));    \
                                           system("poweroff");                                                     \
                                __VA_ARGS__;                                                                       \
                                }                                                                                  \
                                                                                                                   \
                            } while (0);
*/
