#pragma once
//////////////////////////////////////////////////////////////////
#define TYPE(node)      node->type 
#define VALUE(node)     node->value

#define  LEFT(node)   node->left_child
#define RIGHT(node)   node->right_child
//////////////////////////////////////////////////////////////////
#define NewConstNode(value)                NewDefNode (CONSTANT, {.t_constant = value})
#define NewOpNode(op, left, right)         NewDefNode (OPERATOR, {.t_operator = op}, left, right)
#define NewFunctNode(functor, left, right) NewDefNode (FUNCTOR,  {.t_operator = functor}, left, right)
//////////////////////////////////////////////////////////////////
#define TYPE_IS(node, type) ((node) && (node->type == type) ? true : false) 

#define       CONST(node)  node->value.t_constant
#define  LEFT_CONST(node)  CONST( LEFT(node))
#define RIGHT_CONST(node)  CONST(RIGHT(node))

#define       OP(node)     node->value.t_operator
#define  LEFT_OP(node)     OP( LEFT(node))
#define RIGHT_OP(node)     OP(RIGHT(node))

#define       VAR(node)    node->value.t_variable
#define  LEFT_VAR(node)    VAR( LEFT(node))
#define RIGHT_VAR(node)    VAR(RIGHT(node))

#define       FUNC(node)   node->value.t_functor
#define  LEFT_FUNC(node)   FUNC( LEFT(node))
#define RIGHT_FUNC(node)   FUNC(RIGHT(node))
///////////////////////////////////////////////////////////////////
#define OP_IS(op) ( ( (NODE) && (NODE->type == OPERATOR) && (NODE->value.t_operator == op)) ? true : false )

#define       COPY(node)  ( (node) ? CopyBranch(node) : nullptr)
#define  COPY_LEFT(node)  ( (node) ? CopyBranch(node-> left_child) : nullptr)
#define COPY_RIGHT(node)  ( (node) ? CopyBranch(node->right_child) : nullptr)

#define  DIF_LEFT(node) Differentiate (node->left_child,  variable)
#define DIF_RIGHT(node) Differentiate (node->right_child, variable)  
///////////////////////////////////////////////////////////////////
#define IS_OP(node)    ( (node) && (node->type == OPERATOR) ? true : false )
#define IS_CONST(node) ( (node) && (node->type == CONSTANT) ? true : false )
#define IS_VAR(node)   ( (node) && (node->type == VARIABLE) ? true : false )
#define IS_FUNC(node)  ( (node) && (node->type == FUNCTOR ) ? true : false )

#define IS_ZERO(node)  ( IS_CONST(node) && (node->value.t_constant == 0) ? true : false ) 
#define IS_ONE(node)   ( IS_CONST(node) && (node->value.t_constant == 1) ? true : false ) 
///////////////////////////////////////////////////////////////////
#define KILL_BRANCHES(node) do                      \
                        {                           \
                        DeleteBranch( LEFT(node));  \
                        DeleteBranch(RIGHT(node));  \
                        } while(0);     

#include "LogMacroses.h"

#define $LOG_NODE(node) do                                                                          \
                        {                                                                           \
                        logf("DefNode %s\n", #node);                                                \
                        logf("(%p)::::::::::::::::\n", (node));                                       \
                        logf("\t\t  left_child: %p\n",  LEFT((node)));                                \
                        logf("\t\t right_child: %p\n", RIGHT((node)));                                \
                        logf("\t\t        type: ");                                                 \
                         switch ((node)->type)                                                        \
                            {                                                                       \
                            case OPERATOR: logf_ni("OPERATOR | {%c}\n\n",  (node)->value.t_operator);  \
                                        break;                                                      \
                                                                                                    \
                            case VARIABLE: logf_ni("VARIABLE | {%c}\n\n",  (node)->value.t_variable);  \
                                        break;                                                      \
                                                                                                    \
                            case CONSTANT: logf_ni("CONSTANT | {%lg}\n\n", (node)->value.t_constant); \
                                        break;                                                      \
                                                                                                    \
                            case FUNCTOR:  logf_ni("FUNCTOR | {%d}\n\n",   (node)->value.t_functor);    \
                                        break;                                                      \
                            default: logf_ni("UNCKNOWN TYPE\n"); break;                             \
                            }                                                                       \
                        }while(0);
