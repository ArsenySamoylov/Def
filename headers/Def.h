#pragma once
#include "DefTree.h"

enum Functions
    {
    SIN,
    COS,
    LN,
    SQRT,
    };

enum T_OPERATORS
    {
    SUM = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/', 
    POW = '^',
    //////////////
    BRACKET,
    };


const int NUMBER_OF_SUPPORTED_FUNCTIONS = 4;
const char *const SUPPORTED_FUNCTIONS[] = {"sin", "cos", "ln", "sqrt"};
const char OPERATORS[] = "+-*/^";

// static_assert (array[sin] == to_str[sin]);

DefNode* Differentiate   (const DefNode *const def_node, char variable);
DefNode* Simplify (DefNode* def_node);
int DefineVariable (char var, double value, DefNode* def_node);

double CountConstants (DefNode* def_node);
