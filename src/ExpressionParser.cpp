#include "ExpressionParser.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "EasyDebug.h"
#include "Def.h"
#include "LogMacroses.h"
#include "my_buffer.h"
#include "Utils.h"

#include "DSL.h"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"

struct TokenBuffer
    {
    Token* arr;

    int size;
    int position;
    };

static DefNode* GetE     (TokenBuffer* token_buf);
static DefNode* GetT     (TokenBuffer* token_buf);
static DefNode* GetPower (TokenBuffer* token_buf);
static DefNode* GetP     (TokenBuffer* token_buf);
static DefNode* GetF     (TokenBuffer* token_buf);
static DefNode* GetN     (TokenBuffer* token_buf);

static int BufferGetWord (Buffer* buf, char* word_buffer);
static int isfunctor (char* str);

#define token  (arr + number_of_tokens)

int Tokenizer (Token** tokens_arr, const char* expression)
    {
    $log(1)
    assertlog (tokens_arr, EFAULT, return LFAILURE);
    assertlog (expression, EFAULT, return LFAILURE);

    Buffer buf{};
    CHECK (BufferCtor(&buf, expression) == SUCCESS, return LFAILURE);

    Token* arr = (Token*) CALLOC (MIN_NUMBER_OF_TOKENS, sizeof(arr[0]));
    if (!arr) return -1;

    int number_of_tokens = 0;
    int size = MIN_NUMBER_OF_TOKENS;

    while (BufferLook(&buf) != '\0')
        {
        // check for resize
        if (number_of_tokens == size)
            {
            size *= 2;
            TODO("RECALLOC\n");
            Token* fuck = (Token*) REALLOC(arr, size * sizeof(arr[0]));  
            if (!fuck) return LFAILURE;
            
            arr = fuck;
            }

        char temp = BufferLook(&buf);
        
        // constant
        if (isdigit(temp) || temp == '-' || temp == '+')
            {
            double const_val = NAN;
            int status = BufferGetDouble(&buf, &const_val);
            if (status == SUCCESS)
                {
                 TYPE(token) = CONSTANT;
                CONST(token) = const_val;

                number_of_tokens++;

                continue;
                }
            }
        
        //operator
        if (strchr(OPERATORS, temp))
            {
            TYPE(token) = OPERATOR;
              OP(token) = BufferGetCh(&buf);
            
            number_of_tokens++;
            continue;
            }

        // variable or function
        if (isalpha(temp))
            {
            char word[MAX_WORD_LENGTH] = "";
            BufferGetWord (&buf, word);

            if (isfunctor(word) == NOT_A_FUNCTOR)
                {
                TYPE(token) = VARIABLE; 
                 VAR(token) = *word; // for variables, save only first char

                number_of_tokens++;
                continue;
                } 

            TYPE(token) = FUNCTOR;
            FUNC(token) = isfunctor(word);
            
            number_of_tokens++;
            continue;
            }

        // brackets
        if (temp == ')' || temp == '(')
            {
            TYPE(token) = BRACKET;
              OP(token) = BufferGetCh(&buf);
            
            number_of_tokens++;
            continue;
            }
            
        YOU_SHALL_NOT_PASS

        printf("Something went wrong in: %s\n", buf.str);
        KILL((void**)arr);

        return LFAILURE;
        }
        
    *tokens_arr = (Token*) REALLOC (arr, number_of_tokens * sizeof(arr[0]));

    // for log
    $li(number_of_tokens)
    for (int i = 0; i < number_of_tokens; i++)
        {
        $li(i)
        $LOG_NODE(*tokens_arr + i)
        }
    //
    return number_of_tokens; 
    }
#undef token

#define POSITION(tokent_tree) token_buf->position
#define SIZE(token_bud)       token_buf->size
#define token                 ( (POSITION(token_buf) < SIZE(token_buf)) ? (token_buf->arr + token_buf->position) : nullptr )

DefNode* GetG (Token* token_arr, int number_of_tokens)
    {
    $log(DEBUG)
    assertlog(token_arr,            EFAULT, return LNULL);
    assertlog(number_of_tokens > 0, EFAULT, return LNULL);

    TokenBuffer token_buf {token_arr, number_of_tokens, 0};

    DefNode*  root = GetE(&token_buf);
    $LOG_NODE(root)

    if (token_buf.position != token_buf.size)
        {
        logf ("Invalid number of tokens: %d (size %d)\n", token_buf.position, token_buf.size); 
        return LNULL;
        }

    return root;
    }


DefNode* GetE (TokenBuffer* token_buf)
    {
    $log(DEBUG_ALL)
    assertlog (token_buf, EFAULT, return LNULL);

    DefNode* node = GetT(token_buf);

    DefNode* prev_op = node;
    while (IS_OP(token) && (OP(token) ==  '+' || OP(token) == '-'))
        { 
        $LOG_NODE(token)
        DefNode* current_op = token;
        POSITION(token_buf)++;

         LEFT(current_op) = prev_op;
        RIGHT(current_op) = GetT(token_buf);

        prev_op = current_op;
        }

    return prev_op;
    } 

DefNode* GetT (TokenBuffer* token_buf)
    {
    $log(DEBUG_ALL)
    assertlog (token_buf, EFAULT, return LNULL);

    DefNode* node = GetPower(token_buf);
    $LOG_NODE(node)

    DefNode* prev_op = node;
    while (IS_OP(token) && (OP(token) ==  '*' || OP(token) == '/'))
        {
        $LOG_NODE(token)
        DefNode* current_op = token;
        POSITION(token_buf)++;
        
         LEFT(current_op) = prev_op;
        RIGHT(current_op) = GetPower(token_buf);

        prev_op = current_op;
        }
    
    return prev_op;
    }

DefNode* GetPower (TokenBuffer* token_buf)
    {
    $log(DEBUG_ALL)
    assertlog (token_buf, EFAULT, return LNULL);

    DefNode* node = GetP(token_buf);

    DefNode* prev_op = node;
    while (IS_OP(token) && OP(token) ==  '^')
        {
        $LOG_NODE(token)
        DefNode* current_op = token;
        POSITION(token_buf)++;

         LEFT(current_op) = prev_op;
        RIGHT(current_op) = GetP(token_buf);

        prev_op = current_op;
        }
    
    return prev_op;
    }

DefNode* GetP (TokenBuffer* token_buf)
    {
    $log(DEBUG_ALL)
    assertlog (token_buf, EFAULT, return LNULL);

    if (TYPE(token) == BRACKET)
        {
        if (OP(token) != '(')
            {
            printf("Missing opening bracket (token position %d)\n", POSITION(token_buf));
            return LNULL;
            }

        POSITION(token_buf)++;

        DefNode* expression = GetE(token_buf); 

        if (OP(token) != ')')
            {
            printf("Missing closing bracket (token position %d)\n", POSITION(token_buf));
            return LNULL;
            }

        POSITION(token_buf)++;
        
        return expression; 
        }

    if (IS_FUNC(token))
        return GetF(token_buf);

    return GetN(token_buf);
    }

static DefNode* GetF (TokenBuffer* token_buf)
    {
    assertlog (token_buf, EFAULT, return LNULL);
    $log(DEBUG_ALL)

    if (!IS_FUNC(token))
        {
        printf ("Ebat, bratic this is not a functor! (token position %d)\n", POSITION(token_buf));
        return LNULL;
        }
    
    DefNode* result = token;
    
    POSITION(token_buf)++;
    
    RIGHT(result) = GetP(token_buf);

    $LOG_NODE(result)
    return result;
    }

DefNode* GetN (TokenBuffer* token_buf)
    {
    $log(DEBUG_ALL)
    assertlog(token_buf, EFAULT, return LNULL);

    if (!IS_CONST(token) && !IS_VAR(token))
        {
        printf("Cringe, this is not variable or constant node =( (token position %d)\n", POSITION(toke_tree));
        return LNULL;
        } 
    
    DefNode* result = token;
    POSITION(token_buf)++;

    $LOG_NODE(result)
    return result;
    }

///////////////////////////////////////////////////////////////////////////////////////////////////
static int isfunctor (char* str)
    {
    assert(str);

    for (int i = 0; i < NUMBER_OF_SUPPORTED_FUNCTIONS; i++)
        if (!stricmp(str, SUPPORTED_FUNCTIONS[i]))
            return i;
    
    return NOT_A_FUNCTOR;
    }

static int BufferGetWord (Buffer* buf, char* word_buffer)
    {
    assertlog(buf,         EFAULT, return LFAILURE);
    assertlog(word_buffer, EFAULT, return LFAILURE);

    buf->str = SkipSpaces(buf->str); 

    int n = 0;
    sscanf(buf->str, "%[a-zA-Z]%n", word_buffer, &n);
    buf->str = SkipSpaces(buf->str + n); 

    // $s(buf->str)
    // $s(word_buffer)
    // $i(n)
    // $$

    return n;
    }