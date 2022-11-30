#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "DefTree.h"

#include "GVConfig.h"

FILE* OpenDotFile  (const char* path);
void  CloseDotFile ();

const char* MakeImg (const char* img_name, const DefNode *const node);