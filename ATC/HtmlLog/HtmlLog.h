#pragma once

#include <stdlib.h>
#include <stdio.h>

void  CloseHtmlLog ();
FILE* OpenHtmlLog  ();

void hlog (const char* format, ...);

int AddImage (const char* path);
