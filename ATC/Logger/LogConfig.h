#pragma once

const char STD_LOG_NAME[] = "./logs/stdlog.ars";

#ifndef LOG_LEVEL
#define LOG_LEVEL (0)
#endif

enum LoggingLevels
    {
    FULL_MAXIMUM_ULTRA_TOTAL_FUCKING_LOGGING = 0,

    DEBUG = 1,
    /////////
    RELEASE = 666,
    };

#include <errno.h>
//  USEFULL ERRNO CODES:
//!         -) EFAULT
//!         -) ENOENT
//!         -) EINVAL
