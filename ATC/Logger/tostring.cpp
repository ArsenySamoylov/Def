#include "tostring.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "EasyDebug.h"

const int MAX_LENGTH_INT = 10;

char* tostring(const char* value)
    {
    return strdup (value);
    }

char* tostring(int value)
    {
    char* buffer = (char*) calloc (MAX_LENGTH_INT + 32, sizeof(buffer[0]));
    if (!buffer)
        {
        printf("PRINTF BESIT CALLOC (%s)\n", __func__);
        
        return NULL;
        }
    // $i(value)
    // $p(buffer)
    snprintf (buffer, MAX_LENGTH_INT, "%d", value);
    $$
    $s(buffer)

    return buffer;
    }