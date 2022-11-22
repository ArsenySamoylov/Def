 #include "DefTree.h"

#include <malloc.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <math.h> // for NAN only

#include "ArsLib.h"

#pragma GCC diagnostic ignored "-Wconversion" // for BufferGetChar

// #define CHECK_CALLOC(ptr, ) 
                            // if(!ptr)
                                // {
                                // logf("Calloc couldn't allocate memmory
enum
    {
    TWO_CHILDREN,
    ONE_CHILD,
    CHILD_FREE,
    };

const char OPERATORS[] = "+-/*"; 

const int   NUMBER_OF_FUNCTIONS = 6;
const char* SUPPORTED_FUNCTIONS[NUMBER_OF_FUNCTIONS] = {
                                     "cos",
                                     "sin",
                                     "arcsin",
                                     "arccos",
                                     "tg",
                                     "ctg",
                                    };

#define CringeScanf(format, ...)                                           \
        {                                                                  \
        int n = 0;                                                         \
        sscanf (buf->str, format "%n", __VA_ARGS__ __VA_OPT__(,) &n);      \
                                                                           \
        buf->str += n;                                                     \
        buf->str = SkipSpaces (buf->str);                                  \
        }

static DefNode* NewDefNode();
static DefNode* ExpressionParser    (Buffer* buf);

static int DeleteBranch   (DefNode* root);
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

static DefNode* NewDefNode()
    {
    // $log(DEBUG)

    DefNode* new_defnode = (DefNode*) calloc (1, sizeof(DefNode));
    CHECK (new_defnode, return logf("Couldn't allocate memory for new node\n"), LNULL);

    new_defnode-> parent     = nullptr;
    new_defnode-> left_child = nullptr;
    new_defnode->right_child = nullptr;

    return new_defnode;
    }

static DefNode* ExpressionParser (Buffer* buf)
    {
    $log(DEBUG)

    assertlog (buf, EINVAL, return LNULL);

    $ls(buf->str)

    char temp = BufferGetCh(buf); 
    $lc(temp)

    if (temp != '(')
        {
        flog << "DYRACHOC, SYNTAX ERROR (missing opening bracket)";
        return LNULL;
        }
    
    DefNode* current_defnode = NewDefNode();
    $lp(current_defnode); flog<<"\n";

    temp = BufferGetCh(buf);
           BufferUngetCh(buf);

    if (temp == '(')
        {
        current_defnode->left_child = ExpressionParser(buf);
        current_defnode->left_child->parent = current_defnode;

        $lp(current_defnode->left_child)

        temp = BufferGetCh(buf);
        BufferUngetCh(buf);
        $lc(temp)
        }
    
    if (temp != '(')
        {
        int status = SetDefNodeValue(current_defnode, buf);
        
        if (status == CHILD_FREE)
            {            
            if (BufferGetCh(buf) != ')')
                return logf("Missing closing braket in %s", buf->str), LNULL;
            
            logf("return CHILD_FREE defnode %p\n", current_defnode);

            return current_defnode;
            }
        }

    current_defnode->right_child = ExpressionParser(buf);
    current_defnode->right_child->parent = current_defnode;
    // $ls(buf->str)
    
    $lp(current_defnode->left_child)
    $lp(current_defnode->right_child)

    if (BufferGetCh(buf) == ')')
        return logf("ret current_defnode\n"), current_defnode;

    logf("Ups, smthg went wrong in - %s (%s:%d)", buf->str, __PRETTY_FUNCTION__, __LINE__);
    return LNULL;
    }

int CloseDefTree (DefTree* tree)
    {
    assertlog(tree, EFAULT, exit(LFAILURE));

    if (tree->status != ACTIVE)
        return MsgRet(LFAILURE, "Ebat, deleting, not active mode\n");
    
    int status = DeleteBranch(tree->root);
    if (status != SUCCESS)
        return MsgRet(FAILURE, "Ebat, error deleting main branch\n");
    
    free((void*)tree->buffer);

    tree->root   = NULL;
    tree->buffer = NULL;
    tree->status = DEAD;

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

static int SetDefNodeValue (DefNode* defnode, Buffer* buf)
    {
    $log(DEBUG)

    assertlog(defnode, EFAULT, return LFAILURE);
    assertlog(buf,     EFAULT, return LFAILURE);

    $ls(buf->str)
    char ch = BufferGetCh(buf);
    $lc(ch)
    
    if (ch == '(' || ch == ')')
        {
        flog << "I CANT WORK WITH BRACKETS !";

        return SYNTAX_ERROR;
        }

    // FUNCTION OR VARIABLE
    if (isalpha(ch))
        {
        char temp = BufferGetCh(buf);
        $lc(temp)

        if  (temp == ')')  
            {
            BufferUngetCh(buf);

            defnode->def_type = VARIABLE;

            defnode->value.t_variable = ch;

            
            logf("ONE CHILD: %c - variable\n", ch);
            $ls(buf->str)
            return CHILD_FREE;
            }

        BufferUngetCh(buf);

        char function[FUNCTION_LENGTH] = "";

        int n = 0;
        sscanf(buf->str, "%[^)]%n)", function, &n);
        buf->str += n; 

        int func_num = isfunction(function);
        if (func_num == NOT_A_FUNCTION)
            {
            logf("Uncknown function %s\n", function);
            return SYNTAX_ERROR;
            }

        defnode->value.t_function = func_num;

        // how many arguments functios have ?
        printf ("@todo: function arguments (%s)\n", function);

        logf("ONE CHILD: %s - function\n", function);
        return ONE_CHILD;
        }
    // CONSTANT
    const char* t_operator = strchr(OPERATORS, ch);

    if (isdigit(ch))
        goto IS_CONSTANT;

    // OPERATOR OR CONSTANT

    if (!t_operator)
        {
        logf("Not a operator: %c\n", ch);
        
        return SYNTAX_ERROR;
        }
    
    if (isdigit(BufferGetCh(buf)))
        {
        IS_CONSTANT:

        BufferUngetCh(buf);
    
        defnode->def_type = CONSTANT;

        double temp = NAN;
        CringeScanf("%lg", &temp);
        // BufferUngetCh(buf); // to return ')'
        
        defnode->value.t_constant = temp;

        logf("CHILD FREE: %lg - constant\n", temp);
        return CHILD_FREE;
        }
    
    BufferUngetCh(buf);

    defnode->def_type = OPERATOR;

    defnode->value.t_operator = *t_operator;

    logf("TWO_CHILDREN: %c - operator\n", *t_operator);
    return TWO_CHILDREN;
    }    

#undef CringeScanf


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define logTODO logf("@todo %s:%d", __func__, __LINE__)

DefNode* Differentiate (DefNode* defnode)
    {
    assertlog (defnode, EFAULT, return NULL);

    if (defnode->def_type == CONSTANT)
        {
        defnode->value.t_constant = 0;
        
        return defnode;
        }

    if (defnode->def_type == VARIABLE)
        {
        if (!defnode->left_child || !defnode->right_child)
            return logf("Vatiable defnode has not null children\n"), LNULL;
        
        DefNode* new_defnode = NewDefNode();
        if (!new_defnode) return LNULL;

        new_defnode->def_type = CONSTANT;
        new_defnode->value.t_constant = 1; 

        CHECK(DeleteBranch(defnode) == SUCCESS, return LNULL);

        return new_defnode;
        }

    if (defnode->def_type == OPERATOR)
        {
        switch (defnode->value.t_operator)
            {
            case '+':
            case '-':   defnode->left_child = Differentiate(defnode->left_child);
                        CHECK (defnode->left_child, return LNULL);

                        defnode->right_child = Differentiate(defnode->right_child);
                        CHECK (defnode->left_child, return LNULL);   
                        break;
            
            case '*':   DefNode* left_copy = CopyBranch (defnode->left_child);
                        CHECK (left_copy, return LNULL);
                        
                        DefNode* right_copy = CopyBranch (defnode->right_child);
                        CHECK (right_copy, return LNULL);
            

                        DefNode* left_branch = NewDefNode();
                        CHECK (left_branch, return LNULL);

                        left_branch->def_type         = OPERATOR;
                        left_branch->value.t_operator = '*';

                        left_branch->left_child  = Differentiate (defnode->left_child);
                        left_branch->right_child = left_copy;


                        DefNode* right_branch = NewDefNode();
                        CHECK (left_branch, return LNULL);
                        
                        right_branch->def_type         = OPERATOR;
                        right_branch->value.t_operator = '*';

                        right_branch->left_child  = left_copy;
                        right_branch->right_child = Differentiate (defnode->right_child);

            
                        defnode->value.t_operator = '+';
                        defnode->left_child  = left_branch;
                        defnode->right_child = right_branch;
                        
                        break;
            case '/':   
                        logf("@todo %s%d", __func__, __LINE__);
                        break;
            }
    
    if (defnode->def_type == FUNCTION)
        {
        logTODO;
        return defnode;
        }
    



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma GCC diagnostic ignored "-Wunused-function" // temprorary

static int AddChildrenToDefNode (DefNode* defnode);
static DefNode* ParsingErrorMessage (const char* message, const char* buffer); // return value now is uselles

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

static DefNode* ParsingErrorMessage (const char* message, const char* buffer)
    {
    printf ("Error in line : ");
    printl (buffer, '\n');
    printf ("%s", message);

    return NULL;
    }