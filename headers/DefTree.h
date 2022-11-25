#pragma once

enum DefNodeType
    {
    OPERATOR, 
    VARIABLE,
    CONSTANT,
    FUNCTOR,
    // /////////
    NOT_A_TYPE   = -12,
    NOT_A_OPERATOR,
    NOT_A_FUNCTOR = -1,
    };

const int FUNCTION_LENGTH = 16;

union DefNodeValue
    {
    char   t_operator;
    char   t_variable;
    double t_constant;
    int    t_functor;
    };

struct DefNode
    {
    DefNode*  parent;

    DefNode*  left_child;
    DefNode* right_child;

    int type; 

    DefNodeValue value;
    };

struct DefTree
    {
    const char* buffer;

    DefNode* root;

    int status; 
    };

int SetDefTree    (DefTree* def_tree, const char* path);
int CloseDefTree  (DefTree* def_tree);

DefNode* NewDefNode (int      type       = 0,       DefNodeValue value       = {0},
                     DefNode* left_child = nullptr, DefNode*     right_child = nullptr, 
                     DefNode* parent     = nullptr);

DefNode* CopyDefNode (const DefNode* original);
DefNode* CopyBranch  (const DefNode* source);
int      DeleteBranch      (DefNode* def_node);
