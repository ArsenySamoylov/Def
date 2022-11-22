#pragma once
#include "my_buffer.h"

enum DefNodeType
    {
    OPERATOR, 
    VARIABLE,
    CONSTANT,
    FUNCTOR,
    // /////////
    NOT_A_OPERATOR,
    NOT_A_FUNCTOR = -1,
    };

const int FUNCTION_LENGTH = 16;

struct DefNode
    {
    DefNode*  parent;

    DefNode*  left_child;
    DefNode* right_child;

    int type; 

    union
        {
        char   t_operator;
        char   t_variable;
        double t_constant;
        int    t_functor;
        }
    value;
    };

struct DefTree
    {
    const char* buffer;

    DefNode* root;

    int status; 
    };

int SetDefTree    (DefTree* def_tree, const char* path);
int CloseDefTree  (DefTree* def_tree);

// DefNode* BufferToDefTree (const char* buffer);

int GetG (DefTree* def_tree, const char* buffer);