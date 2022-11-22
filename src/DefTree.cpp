#include "DefTree.h"

#include <stdio.h>
#include "LogMacroses.h"

#pragma GCC diagnostic ignored "-Wconversion"
static DefNode* GetE (Buffer* buf);
static DefNode* GetT (Buffer* buf);
static DefNode* GetP (Buffer* buf);
static DefNode* GetN (Buffer* buf);

static DefNode* GetPower (Buffer* buf);

static DefNode* NewDefNode();
static int BufferGetWord (Buffer* buf, char* word_buffer);

int GetG (DefTree* def_tree, const char* buffer)
    {
    $log(DEBUG)
    assertlog(def_tree, EFAULT, return LFAILURE);
    assertlog(buffer,   EFAULT, return LFAILURE);

    Buffer buf{};
    CHECK(BufferCtor (&buf, buffer) == SUCCESS, return LogMsgRet(FAILURE, "Could't set buffer\n"));

    DefNode*  val = GetE(&buf);
    
    if (BufferGetCh(&buf) != '\0')
        {
        logf ("Missing terminator charachter ('\\0') in: %s\n", buf.str);
        return LFAILURE;
        }

    def_tree->buffer = buffer;
    def_tree->root   = val;
    def_tree->status = ACTIVE;
    $li(val)
    return LSUCCESS;
    }

DefNode* GetE (Buffer* buf)
    {
    $log(DEBUG)
    assertlog (buf, EFAULT, return LNULL);

    DefNode* val = GetT(buf);
    $lp(val)

    while (BufferLook(buf) == '+' || BufferLook(buf) == '-')
        {
        char op = BufferGetCh(buf);

        DefNode* val_2 = GetE(buf);
        $lp(val_2)

        DefNode* result = NewDefNode();
        CHECK (result, logf("BAD_CALLOC"), return LNULL);
        
        result->left_child  = val;
        result->left_child->parent  = result;

        result->right_child = val_2;
        result->right_child->parent = result;

        result->type = OPERATOR;
        result->value.t_operator = (op == '+') ? '+' : '-';
        
        $lp(result)
        return result;
        }

    return val;
    } 

DefNode* GetT (Buffer* buf)
    {
    $log(DEBUG)
    assertlog (buf, EFAULT, return LNULL);

    DefNode* val = GetPower(buf);
    $lp(val)
    // $ls(buf->str)

    while (BufferLook(buf) == '*' || BufferLook(buf) == '/')
        {
        char op = BufferGetCh(buf);

        DefNode* val_2 = GetT(buf);
        $lp(val_2)

        DefNode* result = NewDefNode();
        CHECK (result, logf("BAD_CALLOC"), return LNULL);
        
        result->left_child  = val;
        result->left_child->parent  = result;

        result->right_child = val_2;
        result->right_child->parent = result;

        result->type = OPERATOR;
        result->value.t_operator = (op == '*') ? '*' : '/';
        
        $lp(result)
        return result;
        }

    return val;
    }

enum Functions
    {
    SIN,
    COS,
    SQRT,
    };

int NUMBER_OF_SUPPORTED_FUNCTIONS = 3;
const char* SUPPORTED_FUNCTIONS[] = { "sin", "cos", "sqrt"};
#define NOT_A_FUNCTION (-1)

static int GetFunctor (const char* str)
    {
    assertlog(str, EFAULT, exit(LFAILURE));

    for (int i = 0; i < NUMBER_OF_SUPPORTED_FUNCTIONS; i++)
        {
        if (strstr (SUPPORTED_FUNCTIONS[i], str))
            return i;
        }

    return NOT_A_FUNCTION;
    }

static int BufferGetWord (Buffer* buf, char* word_buffer)
    {
    assertlog(buf,         EFAULT, return LFAILURE);
    assertlog(word_buffer, EFAULT, return LFAILURE);

    int n = 0;
    sscanf(buf->str, "%[a-zA-z]%n", word_buffer, &n);
    buf->str += n; 

    // $s(word_buffer)
    // $i(n)
    // $$

    return n;
    }

const int MAX_WORD_LENGTH = 16;

DefNode* GetP (Buffer* buf)  
    {
    $log(DEBUG)
    assertlog (buf, EFAULT, return LNULL);

    DefNode* val = NewDefNode();
    CHECK (val, logf("BAD_CALLOC"), return LNULL);

    if (BufferLook(buf) > 'A' && BufferLook(buf) < 'z')
        {
        char word[MAX_WORD_LENGTH] = "";
        BufferGetWord (buf, word);

        if (GetFunctor(word) == NOT_A_FUNCTION)
            {
            val->type = VARIABLE;
            val->value.t_variable = *word; // for variables, save only first char

            return val;
            } 

        switch (GetFunctor(word))
            {
            case SIN:
            case COS:
            case SQRT:
            default:
                    val->type = FUNCTOR;
                    val->value.t_functor = -666;

                    val->right_child = GetE(buf);
                    val->right_child->parent = val;
                    TODO

                    return val; 
            }
        }

    if (BufferLook(buf) == '(')
        {
        BufferGetCh(buf);

        val = GetE(buf);
        

        if (BufferGetCh(buf) != ')')
            return logf("Missing closing bracket in: %s", buf->str), LNULL;
        }
    else
        val = GetN(buf);

    $lp(val)
    return val;
    }

DefNode*  GetN (Buffer* buf)
    {
    $log(DEBUG)
    assertlog(buf, EFAULT, return LNULL);

    double val = 0;
    if (BufferGetDouble(buf, &val) != SUCCESS)
        {
        logf("Couldn't reasd double constant from: %s\n", buf->str);

        return LNULL;
        }
    $ld(val)

    DefNode* result = NewDefNode();
    CHECK (result, logf("BAD_CALLOC"), return LNULL);

    result->type = CONSTANT;
    result->value.t_constant = val;

    $lp(result)
    return result;
    }

DefNode* GetPower (Buffer* buf)
    {
    $log(DEBUG)
    assertlog (buf, EFAULT, return LNULL);

    DefNode* val = GetP(buf);
    $lp(val)

    if (BufferLook(buf) == '^')
        {
        BufferGetCh(buf);

        DefNode* val_2 = GetP(buf);
        
        DefNode* result = NewDefNode();
        CHECK (result, logf("BAD_CALLOC"), return LNULL);

        result->left_child = val;
        result->left_child->parent  = result;

        result->right_child = val_2;
        result->right_child->parent = result;

        result->type = OPERATOR;
        result->value.t_operator = '^';

        $lp(result)
        return result;
        }

    return val;
    }

static DefNode* NewDefNode()
    {
    // $log(DEBUG)

    DefNode* new_defnode = (DefNode*) calloc (1, sizeof(DefNode));
    CHECK (new_defnode, logf("Couldn't allocate memory for new node\n"), return LNULL);

    new_defnode-> parent     = nullptr;
    new_defnode-> left_child = nullptr;
    new_defnode->right_child = nullptr;

    return new_defnode;
    }