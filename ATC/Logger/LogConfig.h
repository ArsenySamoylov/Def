#pragma once

const char STD_LOG_NAME[] = "./logs/stdlog.ars";

#ifndef LOG_LEVEL
#define LOG_LEVEL (2)
#endif

enum LoggingLevels
    {
    FULL_MAXIMUM_ULTRA_TOTAL_FUCKING_LOGGING = 0,

    DEBUG_ALL = 1,
    DEBUG     = 2,
    INTERMEDIATE = 3,
    /////////
    RELEASE = 666,
    };

#include <errno.h>
//  USEFULL ERRNO CODES:
//!         -) EFAULT
//!         -) ENOENT
//!         -) EINVAL
