#include "Utils.h"
#include "EasyDebug.h"

int MsgRet (int return_value, const char* format, ... )
    {
    va_list ptr;
    va_start(ptr, format);

    vprintf (format, ptr);

    return return_value;
    }

void MsgNoRet (const char* format, ... )
    {
    va_list ptr;
    va_start(ptr, format);

    vprintf (format, ptr);

    return;
    }

const int SYSTEM_COMMAND_LENGTH = 256;

int SYSTEM(const char* format, ...)
    {
    assert(format)

    va_list ptr;
    va_start(ptr, format);

    char      system_command [SYSTEM_COMMAND_LENGTH + 16] = {};
    vsnprintf (system_command, SYSTEM_COMMAND_LENGTH, format, ptr);
    // $s(system_command)

    va_end(ptr);

    return system   (system_command);
    }