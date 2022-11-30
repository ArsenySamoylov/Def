#include "Def.h"
#include <math.h>

#include "LogMacroses.h"
#include "DSL.h"
#define NODE def_node

static DefNode* DefToConst (DefNode *const def_node, double value);
static DefNode* DefReplaceWith (DefNode* destination, DefNode* source);
static DefNode* PromoteChild (DefNode* step_mom, DefNode* son);

// #define DECLARETE_OPERATOR(op, left, right, parent) NewNode(OPERATOR, {.value = op}, left, right, parent)

DefNode* Differentiate (const DefNode *const def_node, char variable)
    {
    $log(2)
    assertlog (def_node, EFAULT, return LNULL);
    assertlog (variable, EFAULT, return LNULL);
    $LOG_NODE(def_node)
    
    DefNode* result = NewDefNode(TYPE(def_node), VALUE(def_node));
        if (!result) return LNULL;

    switch (TYPE(result))
        {
        case CONSTANT:
            return CONST(result) = 0, result;
        
        case VARIABLE:
            {
            if (VAR(result) == variable)
                {
                TYPE (result) = CONSTANT;
                CONST(result) = 1;
                }

            return result;
            }

        case OPERATOR: 
            {
            if (OP_IS(SUM) || OP_IS(SUB))
                {
                 LEFT(result) =  DIF_LEFT(def_node);
                RIGHT(result) = DIF_RIGHT(def_node);

                if (!result->left_child || !result->right_child)
                    return DeleteBranch(result), LNULL;

                 LEFT(result)->parent = result;
                RIGHT(result)->parent = result;

                return result;
                }

            if (OP_IS(MUL))
                {
                OP(result) = '+';

                 LEFT(result) = NewDefNode (OPERATOR, {.t_operator = '*'},  DIF_LEFT(def_node), COPY_RIGHT(def_node), result);
                RIGHT(result) = NewDefNode (OPERATOR, {.t_operator = '*'}, COPY_LEFT(def_node),  DIF_RIGHT(def_node), result); 

                if (!result->left_child || !result->right_child)
                    return DeleteBranch (result), LNULL;

                return result;
                }

            if (OP_IS(DIV))
                {
                DefNode* numerator = NewDefNode (OPERATOR, {.t_operator = '-'}, nullptr, nullptr, result); 
                if (!numerator) 
                    return DeleteBranch(result), LNULL;

                 LEFT(numerator) = NewOpNode  ( '*',  DIF_LEFT (def_node), COPY_RIGHT(def_node)); 
                RIGHT(numerator) = NewOpNode  ( '*', COPY_RIGHT(def_node),  DIF_RIGHT(def_node));

                DefNode* denominator = NewOpNode ( '*', COPY_RIGHT(def_node), COPY_RIGHT(def_node));
                if (!denominator)
                    return DeleteBranch(result), LNULL;

                 LEFT(result) = numerator;
                RIGHT(result) = denominator;
                
                return result;
                }

            if (OP_IS(POW))
                {
                DefNode* power = Simplify (RIGHT(def_node));
                if (!power) return LNULL;

                DefNode* base = Simplify (LEFT(def_node));

                if (IS_CONST(base) && IS_CONST(power))
                    return DefToConst (result, 0);

                if (IS_CONST(base))
                    {
                    OP(result) = '*';

                    DefNode* temp = NewFunctNode(LN, nullptr, NewConstNode(LEFT_CONST(def_node)));

                     LEFT(result) = COPY(def_node); 
                    RIGHT(result) = NewOpNode ('*', temp, DIF_RIGHT(def_node)); 

                    return result;
                    }

                if (IS_CONST(power))
                    {
                    OP(result) = '*';

                    DefNode* temp = NewOpNode ('^', COPY(base), NewConstNode(CONST(power) - 1)); 

                     LEFT(result) =  NewOpNode ('*', COPY(power), temp);  
                    RIGHT(result) =  DIF_LEFT(def_node); // what if DIF(base) ??? 

                    return result;
                    }
                else
                    {
                    OP(result) = '*';

                    DefNode* temp  = NewOpNode ('*', NewOpNode('/', COPY(power)  , COPY(base)), DIF_LEFT(def_node));
                    DefNode* temp2 = NewOpNode ('*', NewFunctNode(LN, nullptr, COPY(base)), DIF_RIGHT(def_node));
                     
                     LEFT(result) = COPY(def_node);
                    RIGHT(result) = NewOpNode ('+', temp, temp2);
                    
                    return result;
                    }

                printf("LOX!\n");
                }
                

            logf ("UNKNOWN operator: def_node(%d).operator = %c\n", def_node, def_node->value.t_operator);
            return LNULL;
            }

        case FUNCTOR:
            {
            switch (FUNC(result))
                {
                case  SIN: 
                    {
                    DefNode* temp = NewOpNode ('*', result, DIF_RIGHT(def_node)); 
                
                     FUNC(result) = COS;
                    RIGHT(result) = COPY_RIGHT(def_node);
                    
                    return temp;
                    }
                case  COS: 
                    {
                    DefNode* temp = NewOpNode ('*', NewOpNode('*', NewConstNode(-1), result), DIF_RIGHT(def_node)); 
                
                     FUNC(result) = SIN;
                    RIGHT(result) = COPY_RIGHT(def_node);
                    
                    return temp;
                    }
                case   LN:
                    {
                    TYPE(result) = OPERATOR;
                      OP(result) = '/';
                    
                     LEFT(result) = DIF_RIGHT(def_node);
                    RIGHT(result) = COPY_RIGHT(def_node);
                    
                    return result;
                    }
                case SQRT:
                    {
                    TYPE(result) = OPERATOR;
                      OP(result) = '/';
                    
                    DefNode* temp = NewOpNode ('*', NewConstNode(2), NewFunctNode(SQRT, nullptr, COPY_RIGHT(def_node)));

                     LEFT(result) = DIF_RIGHT(def_node);
                    RIGHT(result) = temp;
                    
                    return result;
                    }

                default: return NULL;
                }

            return LNULL;
            }
        default:
            break;
        }

    logf ("Error, you shouldn't reach this place (%s:%d)\n", __func__, __LINE__);

    return LNULL;
    }

#pragma GCC diagnostic ignored "-Wfloat-equal" // for double to work with zero

DefNode* Simplify (DefNode* def_node)
    {
    $log(3)
    assertlog (def_node, EFAULT, return LNULL);
    $LOG_NODE(def_node)

    if (!LEFT(def_node) || !RIGHT(def_node))
        return def_node;

      LEFT(def_node) = Simplify( LEFT(def_node));
    RIGHT(def_node) = Simplify(RIGHT(def_node));

    // mull pow and div with ZERO
    if (OP_IS(POW) && IS_ZERO(RIGHT(def_node)))
        return DefToConst (def_node, 1); 
        
    if ((OP_IS(MUL) || OP_IS(DIV) || OP_IS(POW)) && 
        (IS_ZERO(LEFT(def_node)) || IS_ZERO(RIGHT(def_node)) ))
        return DefToConst (def_node, 0);

    // mul pow and div with ONE
    if ((OP_IS(MUL) || OP_IS(POW)) && 
        (IS_ONE(LEFT(def_node)) || IS_ONE(RIGHT(def_node)) ))
        return PromoteChild (def_node, (IS_ONE(LEFT(def_node)) == false) ? LEFT(def_node) : RIGHT(def_node));
    
    if (OP_IS(DIV) && IS_ONE(RIGHT(def_node)))
        return PromoteChild (def_node, LEFT(def_node));

    // sum and sub with ZERO
    if ((OP_IS(SUM) || OP_IS(SUB)) && 
        (IS_ZERO(LEFT(def_node)) || IS_ZERO(RIGHT(def_node)) ))
        return PromoteChild (def_node, (IS_ZERO(LEFT(def_node)) == false) ? LEFT(def_node) : RIGHT(def_node));
    
    //COUNT CONSTANTS
    if (IS_CONST(LEFT(def_node)) && IS_CONST(RIGHT(def_node)))
        {
        switch (def_node->value.t_operator)
            {
            case SUM: return DefToConst (def_node, LEFT_CONST(def_node) + RIGHT_CONST(def_node));
            case SUB: return DefToConst (def_node, LEFT_CONST(def_node) - RIGHT_CONST(def_node));
            case MUL: return DefToConst (def_node, LEFT_CONST(def_node) * RIGHT_CONST(def_node));
            case DIV: return DefToConst (def_node, LEFT_CONST(def_node) / RIGHT_CONST(def_node));
            case POW: return DefToConst (def_node, LEFT_CONST(def_node) + RIGHT_CONST(def_node));
            default:  return logf("Error: trying to simplify unckown operator (%c)\n", def_node->value.t_operator), LNULL;
            }
        } 

    //  LEFT(def_node) = Simplify( LEFT(def_node));
    // RIGHT(def_node) = Simplify(RIGHT(def_node));

    return def_node;
    }

/////////////////////////////////////////////////////////////////////////////
int DefineVariable (char var, double value, DefNode* def_node)
    {
    $log(1)
    if (!def_node) return 0;

    assertlog(var, EFAULT, return LFAILURE);
    $LOG_NODE(def_node)

    if (IS_VAR(def_node) && VAR(def_node) == var)
        {
        if (!RIGHT(def_node))
            RIGHT(def_node) = NewDefNode(CONSTANT, {.t_constant = value});  
        else
            RIGHT_CONST(def_node) = value;

        return LSUCCESS;
        }

    DefineVariable(var, value,  LEFT(def_node));
    DefineVariable(var, value, RIGHT(def_node));

    return 0;
    }

double CountConstants (DefNode* def_node)
    {
    $log(1)
    assertlog(def_node, EFAULT, return LFAILURE);
    $LOG_NODE(def_node)

    if (IS_CONST(def_node))
        return CONST(def_node);

    if (IS_VAR(def_node))
        {
        if (!RIGHT(def_node))
            {
            $LOG_NODE(def_node)
            printf("Variable %c isn't defined\n", VAR(def_node));
              logf("Variable %c isn't defined\n", VAR(def_node));
            
            throw "UNDEFINIED_VARIABLE";
            }
        
        return RIGHT_CONST(def_node);
        }

    if (IS_OP(def_node))
        {
        switch (def_node->value.t_operator)
            {
            case SUM: return CountConstants(LEFT(def_node)) + CountConstants(RIGHT(def_node));
            case SUB: return CountConstants(LEFT(def_node)) - CountConstants(RIGHT(def_node));
            case MUL: return CountConstants(LEFT(def_node)) * CountConstants(RIGHT(def_node));
            case DIV: return CountConstants(LEFT(def_node)) / CountConstants(RIGHT(def_node));
            case POW: return pow (CountConstants(LEFT(def_node)), CountConstants(RIGHT(def_node)));
            default:  throw "Error: trying to simplify unckown operator\n";
            }
        }

    if (IS_FUNC(def_node))
        {
        switch (FUNC(def_node))
            {
            case  SIN: return  sin(CountConstants(RIGHT(def_node)));
            case  COS: return  cos(CountConstants(RIGHT(def_node)));
            case   LN: return  log(CountConstants(RIGHT(def_node)));
            case SQRT: return sqrt(CountConstants(RIGHT(def_node)));
            default: 
                printf("Uncknown functor (%d)\n", FUNC(def_node));
            }

        return NAN;
        } 
    
    YOU_SHALL_NOT_PASS
    throw "You can't reach this place\n";
    }


static DefNode* DefToConst (DefNode *const def_node, double value)
    {
    $log(2)
    assertlog(def_node, EFAULT, return LNULL)
    $LOG_NODE(def_node)

    KILL_BRANCHES(def_node);          
                                                      
    TYPE(def_node)  = CONSTANT;        
    CONST(def_node) = value; 

     LEFT(def_node) = nullptr;
    RIGHT(def_node) = nullptr;
    
    return def_node;
    }

static DefNode* DefReplaceWith (DefNode* destination, DefNode* source)
    {
    $log(0)

    assertlog (destination,   EFAULT, return LNULL);
    assertlog (source, EFAULT, return LNULL);

    $LOG_NODE(destination)
    $LOG_NODE(source)

    destination->parent = source->parent;
    destination->value  = source->value;
    destination->type   = source->type;

    LEFT (destination) = LEFT (source);
    RIGHT(destination) = RIGHT(source);

    KILL( (void**) &source);

    return destination;
    }

static DefNode* PromoteChild (DefNode* step_mom, DefNode* son)
    {
    $log(0)
    assertlog (step_mom, EFAULT, return LNULL);
    assertlog (son,      EFAULT, return LNULL);

    $LOG_NODE(step_mom)
    $LOG_NODE(son)

    if (son != LEFT(step_mom) && son != RIGHT(step_mom))
        return logf("son (%p) is not child of step_mom(%p)\n", son, step_mom), LNULL;

    DeleteBranch ( (son != LEFT(step_mom)) ? LEFT(step_mom) : RIGHT(step_mom));

    return DefReplaceWith (step_mom, son);
    }