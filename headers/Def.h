#pragma once

enum Functions
    {
    SIN,
    COS,
    SQRT,
    };
    
enum T_OPERATORS
    {
    SUM = '+',
    SUB = '-',
    MUL = '*', 
    POW = '^',
    };

const int NUMBER_OF_SUPPORTED_FUNCTIONS = 3;
const char *const SUPPORTED_FUNCTIONS[] = { "sin", "cos", "sqrt"};
#define NOT_A_FUNCTION (-1)
