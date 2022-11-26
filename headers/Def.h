#pragma once
#include "DefTree.h"

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
    DIV = '/', 
    POW = '^',
    };


const int NUMBER_OF_SUPPORTED_FUNCTIONS = 3;
const char *const SUPPORTED_FUNCTIONS[] = {"sin", "cos", "sqrt"};
#define NOT_A_FUNCTION (-1)

// static_assert (array[sin] == to_str[sin]);

DefNode* Differentiate   (const DefNode *const def_node, char variable);
DefNode* Simplify (DefNode* def_node);
// DefTree* SimplifyDefTree (DefNode* def_node);
int DefineVariable (char var, double value, DefNode* def_node);

double CountConstants (DefNode* def_node);
