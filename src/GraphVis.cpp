#include "GraphVis.h"

#include <assert.h>
#include <stdarg.h>

#include "LogMacroses.h"

static FILE* Dot = NULL; 

const char* MakeImg (const char* img_name, const BinaryTree* data_tree) 
    {
    assert (img_name);

    char      dot_file [MAX_GRAPH_NAME_LENGTH + 16] = {};
    snprintf (dot_file, MAX_GRAPH_NAME_LENGTH, "%s%s.dot", 
                                                PATH_FOR_DOT, img_name);
    //printf   ("(%s)\n", dot_file);
    
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

void DotTreeBranch (Node* node)
    {
    assertlog (node, EFAULT, abort());
    
    PrintNode (node);
    
    if (node->first_child)
        {
        PrintNode (node->first_child);

        DotPrint ("\t\tnode_%p -> node_%p [label = \"Yes\", color = Green]\n", node, node->first_child);

        DotTreeBranch (node->first_child);
        }

    if (node->second_child)
        {
        PrintNode (node->second_child);

        DotPrint ("\t\tnode_%p -> node_%p [label = \"NO\", color = Red]\n", node, node->second_child);

        DotTreeBranch (node->second_child);
        }

    return;
    }


void PrintNode (Node* node)
    {
    assertlog (node, EFAULT, abort());
    
    DotPrint ("\t\tnode_%p [shape = Mrecord label =  \"%s\"]\n", node, node->data);

    return;
    }    