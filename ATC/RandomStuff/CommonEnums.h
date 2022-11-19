#pragma once

const int CRINGE = 0xDED;

enum ReturnStatus
    {
    SUCCESS = 0,
    FAILURE = -1, 
    };

enum ObjectStatus
    {
    UNITIALIZED =  0,
    ACTIVE      =  2, 
    DEAD        =  3,
    };

enum ArgumentsErrors
    {
    NO_ARGUMENTS                = 4,
    WRONG_ARGUMENT              = 5,  
    WROMG_NUMBER_OF_ARGUMENTS   = 6,
    };

enum FILEerrors
    {
    ERROR_OPENING_FILE = 7,
    ERROR_READING_FILE = 8,
    };

enum DynamicMemoryErrors
    {    
    BAD_CALLOC   = 9,
    BAD_REALLOC  = 10,
    };

enum CommonErrors
    {
    NULL_PTR               = 11,
    NULL_STRUCTURE_MEMBER  = 12,

    SYNTAX_ERROR           = 13,
    LOX                    = 0xDED,  
    };
