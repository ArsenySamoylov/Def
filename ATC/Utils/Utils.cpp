#include "Utils.h"

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