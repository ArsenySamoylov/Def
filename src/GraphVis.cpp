#include "DefGraphVis.h"

#include <stdarg.h>

#include "EasyDebug.h"
#include "LogMacroses.h"

static FILE* Dot = NULL; 

const char* MakeImg (const char* img_name, const DefTree* data_tree) 
    {
    $log(2)
    assert (img_name);

    char      dot_file [MAX_GRAPH_NAME_LENGTH + 16] = {};
    snprintf (dot_file, MAX_GRAPH_NAME_LENGTH, "%s%s.dot", 
                                                PATH_FOR_DOT, img_name);
    // printf   ("(%s)\n", dot_file);
    // $$
    
    OpenDotFile (dot_file);
    if (!Dot) return NULL;

    SetDot ();

    // THIS COULD BE THE PLACE FOR YOUR FUNCTION
    
    DotTreeBranch (data_tree->root);
    // FOR EXample like this ^

    SetEndDot ();

    static char full_img_name [IMAGE_NAME_LENGTH + 16] = {}; // why static ???? to return its address dumbass
    snprintf   (full_img_name, IMAGE_NAME_LENGTH, "%s%s.%s", 
                                                   PATH_FOR_IMG, img_name, EXTENSION);
   
    char      system_command [SYSTEM_COMMAND_LENGTH + 16] = {};
    snprintf (system_command, SYSTEM_COMMAND_LENGTH, "dot -T %s -o %s %s", EXTENSION, full_img_name, dot_file);
    //$s(system_command)

    system   (system_command);
    
    CloseDotFile ();
    return full_img_name;
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////
void SetDot ()
    {
    if (!Dot) return;

    set_func_name;

    DotPrint("digraph \n{\n");
    DotPrint("rankdir = %s # RANKDIR\n", RANKDIR);

    return;
    }

void SetEndDot ()
    {
    if (!Dot)   return;

    set_func_name;

    DotPrint("\n}"); 
    fflush(Dot);

    return;   
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////
void DotPrint (const char* format, ...)
    {
    va_list ptr;
    va_start(ptr, format);

    vfprintf (Dot, format, ptr);

    va_end(ptr);
    
    return;
    }

FILE* OpenDotFile (const char* path)
    {
    if (!path)  return NULL;
    return Dot = fopen (path, "wb"); 
    }

void CloseDotFile ()
    {
    fclose (Dot);

    Dot = NULL;
    return; 
    }

void DotTreeBranch (DefNode* node)
    {
    assertlog (node, EFAULT, abort());
    
    PrintNode (node);
    // $p(node->left_child)
    // $p(node->right_child)

    if (node->left_child)
        {
        PrintNode (node->left_child);

        DotPrint ("\t\tnode_%p -> node_%p \n", node, node->left_child);

        DotTreeBranch (node->left_child);
        }

    if (node->right_child)
        {
        PrintNode (node->right_child);

        DotPrint ("\t\tnode_%p -> node_%p \n", node, node->right_child);

        DotTreeBranch (node->right_child);
        }

    return;
    }

#include <string.h>
#include "DSL.h"

void PrintNode (DefNode* node)
    {
    $log(1) 
    assertlog (node, EFAULT, abort());
    $LOG_NODE(node)

    char def_data[333]   = "";


    switch (node->type)
        {
        case OPERATOR: sprintf(def_data, "Operator | {%c}", node->value.t_operator);
                       break;

        case VARIABLE: sprintf(def_data, "Variable | {%c}", node->value.t_variable);
                       break;

        case CONSTANT: sprintf(def_data, "Constant | {%lg}", node->value.t_constant);
                       break;

        case FUNCTOR: sprintf(def_data, "Functor | {%d}", node->value.t_functor);
                       break;
        default: break;
        }

    DotPrint ("\t\tnode_%p [shape = Mrecord label =  \"%s\"]\n", node, def_data);

    return;
    }    