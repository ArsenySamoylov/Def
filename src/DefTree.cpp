#include "DefTree.h"

#include <malloc.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <math.h> // for NAN only

#include "ArsLib.h"

#pragma GCC diagnostic ignored "-Wconversion" // for BufferGetChar
#pragma GCC diagnostic ignored "-Wunused-function" // temprorary

enum
    {
    TWO_CHILDREN,
    ONE_CHILD,
    CHILD_FREE,
    };

const char* SUPPORTED_FUNCTIONS[NUMBER_OF_FUNCTIONS] = {
                                     "cos",
                                     "sin",
                                     "arcsin",
                                     "arccos",
                                     "tg",
                                     "ctg",
                                    };

static DefNode* NewDefNode();
static int DeleteBranch         (DefNode* root);
static int AddChildrenToDefNode (DefNode* defnode);
static void LogDefNod (const DefNode* defnode);
static DefNode* ParsingErrorMessage (const char* message, const char* buffer); // return value now is uselles

static DefNode* ExpressionParser (Buffer* buf);
int GetDefNodeType (Buffer* buf);

static int SetDefNodeValue(DefNode* current_defnode, Buffer* buf);

static int isfunction (const char* str);

int SetDefTree (DefTree* def_tree, const char* path)
    {
    $log(DEBUG)

    assertlog (def_tree,  EFAULT, exit(LFAILURE));
    assertlog (path,      ENOENT, exit(LFAILURE));

    const char* buffer = GetSrcFile(path);
    assertlog (buffer, ENOENT, \
              return  MsgRet (FAILURE, "Can't find data base in path: %s\n", path));  

    Buffer buf{};
    BufferCtor (&buf, buffer);
    $s(buffer);
    DefNode* root = ExpressionParser (&buf);
    CHECK (root, return LFAILURE);
    
    // set def_tree
    def_tree->root   = root;
    def_tree->buffer = buffer;
    def_tree->status = ACTIVE;

    return LogMsgRet (SUCCESS, "Data base setted succesfuly (from path: %s)\n\n", path);
    }


#define CringeScanf(format, ...)                                           \
        {                                                                  \
        int n = 0;                                                         \
        sscanf (buf->str, format "%n", __VA_ARGS__ __VA_OPT__(,) &n);      \
                                                                           \
        buf->str += n;                                                     \
        buf->str = SkipSpaces (buf->str);                                  \
        }

static DefNode* NewDefNode()
    {
    $log(DEBUG)

    DefNode* new_defnode = (DefNode*) calloc (1, sizeof(DefNode));
    CHECK (new_defnode, return LNULL);

    new_defnode-> parent     = nullptr;
    new_defnode-> left_child = nullptr;
    new_defnode->right_child = nullptr;

    return new_defnode;
    }

static DefNode* ExpressionParser (Buffer* buf)
    {
    $log(DEBUG)

    assertlog (buf, EINVAL, exit(LFAILURE));

    $lc(*buf->str)
    $lc(BufferGetCh(buf))
    $$
    // change to != '(' - syntax error
    if (BufferGetCh(buf) == '(')
        {
        $$
        DefNode* current_defnode = NewDefNode();
        
        char temp = BufferGetCh(buf); 
        $lc(temp)
        if (temp != '(' && temp != ')')
            {
            int status = SetDefNodeValue(current_defnode, buf);

            if (status == TWO_CHILDREN)
                {
                current_defnode->right_child = ExpressionParser(buf);
                current_defnode->right_child->parent = current_defnode;
                }

            if (BufferGetCh(buf) != ')')
                return logf("Missing closing braket in %s", buf->str), LNULL;

            return current_defnode;
            }

        BufferUngetCh(buf);

        current_defnode->left_child = ExpressionParser(buf);
        current_defnode->left_child->parent = current_defnode;

        return current_defnode;
        }

    printf("EROROR, YOU COULD'T REACH HERE %s:%d\n", __func__, __LINE__);
    return LNULL;
    }

static int SetDefNodeValue(DefNode* current_defnode, Buffer* buf)
    {
    $log(DEBUG)

    assertlog (current_defnode, EFAULT, exit(LFAILURE));
    assertlog (buf, EINVAL, exit(LFAILURE));
    // mb check something in def_node
    // forexample root should be binary
    // 

    switch(GetDefNodeType(buf))
        {
        case '(':
            {
            logf("Erorr, here should't be opening bracket- %s\n", buf->str);
            // current_defnode-> left_child = ExpressionParser (buf);
            // current_defnode->right_child = ExpressionParser (buf);

            // current_defnode-> left_child->parent = current_defnode;
            // current_defnode->right_child->parent = current_defnode;
            // ((x) * (4))
            break;
            }
        case ')':
            {
            logf("Erorr, here should't be closing bracket- %s\n", buf->str);
            // return current_defnode;

            break;
            }
        case OPERATOR: 
            {
            current_defnode->def_type = OPERATOR;

            current_defnode->value.t_operator = BufferGetCh(buf);

            current_defnode-> left_child = ExpressionParser(buf);
            current_defnode->right_child = ExpressionParser(buf);
            
            return TWO_CHILDREN; 
            break;
            }
        case VARIABLE:
            {
            current_defnode->def_type = VARIABLE;

            current_defnode->value.t_variable = BufferGetCh(buf);

            current_defnode-> left_child = nullptr;
            current_defnode->right_child = nullptr;

            return CHILD_FREE;
            break;
            }
        case FUNCTION:
            {
            current_defnode->def_type = FUNCTION;

            char function[FUNCTION_LENGTH] = "";

            int n = 0;
            sscanf(buf->str, "%[^)]%n)", function, &n);
            buf->str += n; 

            current_defnode->value.t_function = isfunction(function); 

            // how many arguments functios have ?
            printf ("@todo: function arguments (%s)\n", function);

            current_defnode->left_child  = nullptr;
            current_defnode->right_child = nullptr;

            return ONE_CHILD;
            break;
            }
        case CONSTANT:
            {
            current_defnode->def_type = CONSTANT;

            double temp = NAN;
            CringeScanf("%lg", &temp);

            current_defnode->value.t_constant = temp;

            current_defnode->left_child  = nullptr;
            current_defnode->right_child = nullptr;

            return CHILD_FREE;
            break;
            }
        case NOT_A_OPERATOR:
        case NOT_A_FUNCTION:
        default: 
            {
            printf("Syntax error in: (%.15s)\n", buf->str);

            logf("Syntax error in expression (%s)\n", buf->buffer);
            return LFAILURE;
            break;
            }
        }

    printf("EROROR, YOU COULD'T REACH HERE %s:%d\n", __func__, __LINE__);

    return LFAILURE;
    }

/*
static DefNode* SetNewDefNod (DefNode* parent, const char** buffer)
    {
    $log(DEBUG)

    //assert(parent); // PARENT CAN BE NULL !!! 
    assertlog (buffer, EINVAL, return LNULL);

    DefNode* new_defnod = (DefNode*) calloc (1, sizeof(new_defnod[0]));
    CHECK (new_defnod, return LNULL);

    new_defnod->parent = parent;

    *buffer = SkipSpaces (++(*buffer));        

    int n = 0;
    int status = sscanf (++(*buffer), "%[^\"]%n", new_defnod->data, &n);

    if (!status)
        return ParsingErrorMessage ("sscanf could't read this line\n", *buffer);

    if (!n)
        return ParsingErrorMessage ("\"statement\" is empty\n", *buffer);

    if (*(*buffer + n) != '"')
        return ParsingErrorMessage ("\"statement\" missing closig \" bracket \n", *buffer);

    *buffer = SkipSpaces(*buffer + n + 1);

    new_defnod->right_child  = NULL;
    new_defnod->left_child = NULL;

    // current_defnod = new_defnod;
    // number_of_defnods++;

    LogDefNod (new_defnod);

    return new_defnod;
    }

static DefNode* FinishSetting (DefNode* current_defnod, const char** buffer)
    {
    $log(DEBUG)

    assert(current_defnod);
    assert(buffer);

    DefNode* prev_defnod = current_defnod->parent;

    // if prev_defnod is NULL, than current_defnod is root
    if (!prev_defnod)
        {
        *buffer = SkipSpaces(++(*buffer));   

        return current_defnod;
        }

    if (!prev_defnod->right_child)
            prev_defnod->right_child = current_defnod;

    else 

        if (!prev_defnod->left_child)
                prev_defnod->left_child = current_defnod;

        else
            
            {
            printf ("Error in line : ");
            printl ( *buffer, '\n');
            printf ("More specificly: %.20s\n", *buffer);

            printf ("Data tree defnode can't have more then two children\n");

            return NULL;
            }
        
    // (prev_defnod->right_child == NULL) ? prev_defnod->right_child : prev_defnod->left_child = current_defnod;
    LogDefNod (prev_defnod);

    current_defnod = prev_defnod;
    
    *buffer = SkipSpaces(++(*buffer));

    return prev_defnod;
    }
*/
int CloseDefTree (DefTree* tree)
    {
    assertlog(tree, EFAULT, exit(LFAILURE));
    // CHECK_STDERR(tree, return NULL_PTR);

    // if (tree->status != ACTIVE)
    //     return MsgRet(FAILURE, "Ebat, deleting, not active mode\n");
    
    // int status = DeleteBranch(tree->root);
    // if (status != SUCCESS)
    //     return MsgRet(FAILURE, "Ebat, error deleting main branch\n");

    // tree->root   = NULL;
    // tree->size   = 0;
    // tree->status = DEAD;
    printf ("@todo\n");
    return SUCCESS;
    }

static int AddChildren (DefNode* defnode)
    {
    assertlog(defnode, EFAULT, exit(LFAILURE));

    if (defnode->right_child)
        return MsgRet(FAILURE, "DefNode already has first child\n");

    if (defnode->left_child)
        return MsgRet(FAILURE, "DefNode already has second child\n");

    DefNode* right_child  = (DefNode*) calloc (1, sizeof(DefNode));
    CHECK (right_child,  return BAD_CALLOC);

    DefNode* left_child = (DefNode*) calloc (1, sizeof(DefNode));
    CHECK (left_child, return BAD_CALLOC);

    right_child->parent       = defnode;
    right_child->right_child  = NULL;
    right_child->left_child = NULL;

    left_child->parent       = defnode;
    left_child->right_child  = NULL;
    left_child->left_child = NULL;

    defnode->right_child  = right_child;
    defnode->left_child = left_child;

    return LSUCCESS;
    }

static int  DeleteBranch (DefNode* root)
    {
    assertlog(root, EFAULT, exit(LFAILURE));

    if (! (root->right_child || root->left_child) )
        {
        free (root);
        root = NULL;

        return SUCCESS;
        }

    int status  = DeleteBranch (root->right_child);
    if (status != SUCCESS)
        return MsgRet (FAILURE, "Could't delete right_child branch\n");

        status  = DeleteBranch (root->left_child);
    if (status != SUCCESS)
        return MsgRet (FAILURE, "Could't delete left_child branch\n");

    return LSUCCESS;
    }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static DefNode* ParsingErrorMessage (const char* message, const char* buffer)
    {
    printf ("Error in line : ");
    printl (buffer, '\n');
    printf ("%s", message);

    return NULL;
    }

/*
static void LogDefNod (const DefNode* defnode)
    {
    logf ("\nparent: %p\nData: (%s)\nAddress: %p\n\tFChild: %p \t Schild: %p\n", defnode->parent,
                defnode->data, defnode, defnode->right_child, defnode->left_child);

    return;
    }   
*/


static int isfunction (const char* str)
    {
    assertlog(str, EFAULT, exit(LFAILURE));

    for (int i = 0; i < NUMBER_OF_FUNCTIONS; i++)
        {
        if (strstr (SUPPORTED_FUNCTIONS[i], str))
            return i;
        }

    return NOT_A_FUNCTION;
    }

const char OPERATORS[] = "+-/*"; 

int GetDefNodeType (Buffer* buf)
    {
    assertlog(buf, EFAULT, exit(FAILURE));

    char ch = BufferGetCh(buf);

    if (ch == '(' || ch == ')')
        return ch;

    if (isalpha(ch))
        {
        char temp = BufferGetCh(buf);
        if  (temp == ')')
            {
            BufferUngetCh(buf);
            BufferUngetCh(buf);

            return VARIABLE;
            }

        BufferUngetCh(buf);

        char function[FUNCTION_LENGTH] = "";

        int n = 0;
        sscanf(buf->str, "%[^)]%n)", function, &n);
        //buf->str += n; // cause I need to get function later on

        if (isfunction(function))
            return FUNCTION;

        return NOT_A_FUNCTION;
        }
                // t_operator
    const char* t_operator = strchr(OPERATORS, ch);

    if (!t_operator)
        return NOT_A_OPERATOR;
    
    if (isdigit(BufferGetCh(buf)))
        return BufferUngetCh(buf), CONSTANT;
    
    return OPERATOR;
    }    

