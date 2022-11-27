#include "Logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <assert.h>

#include "my_buffer.h" // for fsettindent
#include "LinuxColors.h"

// #define CONSOLE(format, ptr)  vprintf(format, ptr)
#define CONSOLE(format, ptr) 
////////////// constructor and destructor ////////////////////////////////////
// @todo try and catch
Logger::Logger (const char* log_name) : my_log(NULL), level(0), indent(0), logname(log_name) 
            {
            try
               {
               my_log = fopen (logname, "w"); 

               if (!my_log)
                    throw "Ebat, lox, You can't even open logfile\n";

               setvbuf(my_log, NULL, _IONBF, 0);

               time_t NewYorkTime = time(NULL);
               fprintf (my_log, "%s\n", ctime(&NewYorkTime));
               }
            catch(const char* message)
               {
               perror (message);
               }
            }

            Logger& Logger::getInstance()
            {
            static Logger logger{STD_LOG_NAME}; // this way you could have only one Logger

            return logger;//ars::Logger &log = ars::Logger::getInstance();
            }

Logger:: ~Logger()
    {
    time_t NewYorkTime = time(NULL);

    log("*********************************************************************\n");
    log("\t\tLog Successfuly ended, on %s", ctime(&NewYorkTime));
    log("*********************************************************************\n");

    fclose(my_log);    
    my_log = NULL;

    printf (cyancolor "Log closed\n" resetconsole);
    // return;
    }
/////////////////////////////////////////////////////////////////////////////
////////////// work with class members //////////////////////////////////////
int Logger::getlevel ()
    { return level;}

FILE* Logger::getlog ()
    { return my_log; }

int Logger::getindent ()
    { return indent; }

int Logger::setlevel (int lvl)
    {
    if (lvl >= 0)
        level = lvl;

    return level;
    }

int Logger::setindent (int indnt)
    {
    if (indnt >= 0)
        indent = indnt;

    return indent;
    }

/////////////////////////////////////////////////////////////////////////////
////////////// new usefull functions ////////////////////////////////////////
void Logger::log (const char* format, ...)
    {
    if (level < LOG_LEVEL)
        return;

    va_list ptr;
    va_start(ptr, format);

    fsetindent (my_log, indent);
    vfprintf   (my_log, format, ptr);

    CONSOLE (format, ptr);

    va_end(ptr);

    return;
    }

void Logger::log_no_indent (const char* format, ...)
    {
    if (level < LOG_LEVEL)
        return;

    va_list ptr;
    va_start(ptr, format);

    // fsetindent (my_log, indent);
    vfprintf   (my_log, format, ptr);

    CONSOLE (format, ptr);

    va_end(ptr);

    return;
    }
    
void Logger::logmsg (char* message)
    {
    assert(message);

    fprintf (my_log, "%s\n", message);

    return;
    }

void* Logger::CAL_LOG (size_t number_of_elmts, size_t size_of_elmts, const char* file, const char* func, int line)
    {
    void* ptr =  calloc (number_of_elmts, size_of_elmts); 

    Logger::getInstance().log ("LOG CALLOC:     {%p} = calloc (%zu, %zu = %zu) from %s:%d, at %s:%d\n", ptr, number_of_elmts, size_of_elmts,
                                                             number_of_elmts * size_of_elmts, 
                                                             func, line, file, line);     
    if (!ptr)
        {
        Logger::getInstance().log("LOG CALLOC: Couldn't allocate memory\n");
        printf("LOG CALLOC: Couldn't allocate memory of size %zu (%zu*%zu) at %s:%d, %s:%d\n", number_of_elmts * size_of_elmts, number_of_elmts, size_of_elmts,
                                                              func, line, file, line);
        }

    return ptr;
    } 

void Logger::FREE_LOG (void** break_free, const char* file, const char* func, int line)
    {
    if (!break_free)
        {
        Logger::getInstance().log("LOG FREE (kill): Atempt to free null pointer from: %s:%d, at %s:%d\n", func, line, file, line);
                           printf("LOG FREE (kiLL): Atempt to free null pointer from: %s:%d, at %s:%d\n", func, line, file, line);
        
        return;
        }

    Logger::getInstance().log("LOG FREE(kill): {%p} - will be freed from %s:%d, at %s:%d\n", *break_free, func, line, file, line);

    free(*break_free);  

    *break_free = nullptr;

    return;
    }
/////////////////////////////////////////////////////////////////////////////
////////////// old usefull functions ////////////////////////////////////////
int Logger::LogMsgRet(int return_value, const char* format, ... )
    {
    // va_list ptr;
    // va_start(ptr, format);

    // vfprintf (my_log, format, ptr);
    // fprintf  (my_log, "\n");

    va_list ptr;
    va_start(ptr, format);

    fprintf    (my_log, "\n");
    fsetindent (my_log, indent);

    vfprintf (my_log, format, ptr);
    fprintf  (my_log, "(return %d;)", return_value);
   
    fprintf  (my_log, "\n");

    CONSOLE (format, ptr);

    va_end(ptr);

    return return_value;
    }

void Logger::LogMsgNoRet(const char* format, ... )
    {
    va_list ptr;
    va_start(ptr, format);

    fprintf    (my_log, "\n");
    fsetindent (my_log, indent);

    fprintf  (my_log, "return; ");
    vfprintf (my_log, format, ptr);
    fprintf  (my_log, "\n");

    CONSOLE (format, ptr);

    va_end(ptr);

    return;
    }

std::nullptr_t Logger::LogMsgNullRet(const char* format, ...)
    {
    va_list ptr;
    va_start(ptr, format);

    fprintf    (my_log, "\n");
    fsetindent (my_log, indent);

    fprintf  (my_log, "return Null ptr;");
    vfprintf (my_log, format, ptr);
    fprintf  (my_log, "\n");

    CONSOLE (format, ptr);
    va_end(ptr);

    return nullptr;
    }
/////////////////////////////////////////////////////////////////////////////
////////////// old  maybe ??? usefull  ??? functions ////////////////////////
void Logger::logerror (int error_code, const char* format, ...)
    {
    va_list ptr;
    va_start(ptr, format);

    vfprintf (my_log, format, ptr);

    log("\n\terror: (code %d) %s\n\n", error_code, strerror(error_code));                
    
    CONSOLE (format, ptr);

    va_end(ptr);

    return;
    }

void Logger::logerrno (const char* format, ...)
    {
    va_list ptr;
    va_start(ptr, format);

    vfprintf (my_log, format, ptr);

    log("\n\terror: (code %d) %s\n\n", errno, strerror(errno));                

    CONSOLE (format, ptr);
    va_end(ptr);
                                                 
    return;
    }

void Logger::checkerrno (const char* format, ...)
    {
    if (errno)
        {
        va_list ptr;
        va_start(ptr, format);

        vfprintf (my_log, format, ptr);

        log("\n\terror: (code %d) %s\n\n", errno, strerror(errno));                
        } 
    else
        {
        log("Everything fine\n\n");
        }  

    return;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
// void log();

// template<typename Type>
// void test___log(const Type &value);

// template<typename Type, typename... Types>
// void test___log(const Type &val, const Types&... vals)
// {
//     test___log(val);
//     test___log(vals...);
// }
