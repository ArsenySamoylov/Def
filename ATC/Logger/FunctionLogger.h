#pragma once

#include "Logger.h"


// guard set level
// and if you don set level
    // LOG(LEVEL) << "bnub" << ;
    // LOG() << 

class FunctionLogger
    {
    private:
        int old_level;
        
        int guard_level;
        int current_indent;
        const char* function_name;

        FunctionLogger (const FunctionLogger& origin)           = delete;
        FunctionLogger& operator=(const FunctionLogger& origin) = delete;

    public:
        FunctionLogger(int lvl, const char* func_name); 
        ~FunctionLogger();
    };
