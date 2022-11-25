#pragma once

#include <iostream>

#include "LogConfig.h" 
#include "tostring.h"

class Logger : public std::ostream
    {
    private:
        FILE* my_log; 

        int level;
        int indent; //

        const char* logname;

        Logger(const char* log_name); 

        // deleting constructors
        Logger (const Logger& value)              = delete;
        Logger& operator=(const Logger& original) = delete;

    public:
        static Logger& getInstance();
        ~Logger();

        int getlevel ();
        int setlevel (int lvl);

        int getindent ();
        int setindent (int indnt);
        
        FILE* getlog ();
            
        void log           (const char* format, ...);
        void log_no_indent (const char* format, ...);
        void logmsg        (char* message);

        void checkerrno (const char* format, ...);
        void logerror   (int error_code, const char* format, ...);
        void logerrno   (const char* format, ...);


        int   LogMsgRet    (int   return_value, const char* format, ...);
        void  LogMsgNoRet  (const char* format, ...); 
        std::nullptr_t LogMsgNullRet(const char* format, ...);
    };
        

template <typename Type>
Logger& operator<<(Logger& log_out, const Type& value)
         { 
         char* message = tostring(value);

         (log_out).logmsg(message);
         
         free(message);
         return log_out;
         }

//          ///operator<<<INT>(os, obj)
// template <> 
// std::ostream& operator<< <const char*> (std::ostream& log_out, const (const char*) &value)
//          {    
//          dynamic_cast<Logger>(log_out).logmsg( tostring(value) );

//          return log_out;
//          }