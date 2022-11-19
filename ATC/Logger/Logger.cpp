#include "Logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <assert.h>

#include "my_buffer.h" // for fsettindent
#include "LinuxColors.h"

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

    return;
    }

void Logger::logmsg (char* message)
    {
    assert(message);

    fprintf (my_log, "%s\n", message);

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

    fprintf  (my_log, "return %d; - ", return_value);
    vfprintf (my_log, format, ptr);
    fprintf  (my_log, "\n");

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
                                                          
    return;
    }

void Logger::logerrno (const char* format, ...)
    {
    va_list ptr;
    va_start(ptr, format);

    vfprintf (my_log, format, ptr);

    log("\n\terror: (code %d) %s\n\n", errno, strerror(errno));                
                                                          
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
