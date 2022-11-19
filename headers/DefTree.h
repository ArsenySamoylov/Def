#pragma once

const char opening_ch = '(';
const char closing_ch = ')';

enum DefNodeType
    {
    OPERATOR, 
    VARIABLE,
    CONSTANT,
    FUNCTION,
    // /////////
    NOT_A_OPERATOR,
    NOT_A_FUNCTION = -1,
    // OPENING_BRACKET,
    // CLOSING_BRACKET,
    };

const int FUNCTION_LENGTH = 16;

struct DefNode
    {
    DefNode*  parent;

    DefNode* right_child;
    DefNode*  left_child;

    int def_type; 

    union
        {
        char   t_operator;
        char   t_variable;
        double t_constant;
        int    t_function;
        }
    value;
    };

struct DefTree
    {
    const char* buffer;

    DefNode* root;

    int size;
    int status; 
    };

int SetDefTree    (DefTree* def_tree, const char* path);
int CloseDefTree  (DefTree* def_tree);

DefNode* BufferToDefTree (const char* buffer);

// int DefTreeCtor (DefTree* tree);
// int DefTreeDtor (DefTree* tree);

// int UpdateDataBase  (DataBase* def_tree);
// int SetNewDataBase (FILE* new_base, DefNode* defnode);