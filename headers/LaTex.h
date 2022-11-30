#pragma once

#include "DefTree.h"

const char PATH_TO_TEX[] = "UnitTests/LaTex/";
const char PATH_TO_PDF[] = "UnitTests/LaTex/";

const int MAX_LATEX_NAME_LENGTH = 128;
const int MAX_FORMULA_LENGTH    = 128;
const int MAX_PDF_NAME_LENGTH   = 128;

int    OpenTexFile (const char* path);
void  CloseTexFile ();

int AddDefTreeToTex (const DefNode *const node);
int AddMessage   (const char* format, ...);