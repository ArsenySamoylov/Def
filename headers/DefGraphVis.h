#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "DefTree.h"

#include "GVConfig.h"

FILE* OpenDotFile  (const char* path);
void  CloseDotFile ();

const char* MakeImg (const char* img_name, const DefTree* data_tree);

void DotTreeBranch (DefNode* node);
void PrintNode     (DefNode* node);

// mb ask programmer to write his own functions to include it here ????

const char* MakeImg (const char* image_name);

void SetDot    ();

void SetEndDot ();

void DotPrint     (const char* format, ...);

#define set_func_name   fprintf (Dot, "########################################################### %s\n", __func__);
