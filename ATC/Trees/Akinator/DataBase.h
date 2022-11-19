#pragma once

#include <stdio.h>

#include "BinaryTree.h"

//typedef BinaryTree DataTree;
//! @todo satus that new nodes apeared to decide whenether upload data base or not
struct DataBase
    {
    const char* buffer;

    BinaryTree* data_tree; // pointer or not ??? this is the question

    int status; // = UNITIALIZED;
    };

int OpenDataBase    (DataBase* data_base, const char* path);
int UpdateDataBase (DataBase* data_base);

int SetNewDataBase (FILE* new_base, Node* node);

int BufferToTreeDataBase (BinaryTree* data_tree, const char* buffer);