#include "Def.h"
#include "DefTree.h"

#include <stdio.h>
#include "LogMacroses.h"

DefNode* NewDefNode (int      type,       DefNodeValue value, 
                     DefNode* left_child, DefNode*     right_child, 
                     DefNode* parent)
    {
    $log(DEBUG_ALL)

    DefNode* new_defnode = (DefNode*) calloc (1, sizeof(DefNode));
    CHECK (new_defnode, logf("Couldn't allocate memory for new node\n"), return LNULL);

    new_defnode-> parent     = parent;
    new_defnode-> left_child = left_child;
    new_defnode->right_child = right_child;

    new_defnode->type  = type;
    new_defnode->value = value;

    return new_defnode;
    }

DefNode* CopyDefNode (const DefNode* original)
    {
    $log(DEBUG_ALL)

    assertlog (original, EFAULT, return LNULL);

    DefNode* copy = NewDefNode ();
    if (!copy)
        return logf("Couldn't copy (%p) node\n", original), LNULL;

    copy->parent      = original->parent;
    copy->left_child  = original->left_child;
    copy->right_child = original->right_child;

    copy->type  = original->type;
    copy->value = original->value;

    return copy;
    }

DefNode* CopyBranch (const DefNode* source)
    {
    $log(DEBUG)
    assertlog (source, EFAULT, return LNULL);

    DefNode* new_defnode = NewDefNode(source->type, source->value);

    if (source->left_child != 0)
        new_defnode->left_child = CopyBranch (source->left_child);
    
    if (source->right_child != 0)
        new_defnode->right_child = CopyBranch (source->right_child);

    if (!new_defnode-> left_child && source-> left_child)
        return DeleteBranch(new_defnode), LNULL;

    if (!new_defnode->right_child && source->right_child)
        return DeleteBranch(new_defnode), LNULL;

    return new_defnode;
    }
    
int CloseDefTree (DefTree* def_tree)
    {
    $log(DEBUG_ALL)
    assertlog (def_tree, EFAULT, return LFAILURE);

    if (def_tree->status != ACTIVE)
        return logf("Error, closing not ACTIVE tree\n"), FAILURE;

    if (DeleteBranch(def_tree->root) != SUCCESS)
        return logf("Could't delete def_tree root\n"), FAILURE;

    // free ((void*) def_tree->buffer);

    return LSUCCESS;
    }

int DeleteBranch (DefNode* def_node)
    {
    $log(DEBUG_ALL)
    assertlog (def_node, EFAULT, return LFAILURE);

    if (def_node->left_child != 0)
        CHECK (DeleteBranch (def_node->left_child)  == SUCCESS, return LFAILURE);

    if (def_node->right_child != 0)
        CHECK (DeleteBranch (def_node->right_child) == SUCCESS, return LFAILURE);

    free(def_node);

    return SUCCESS;
    }