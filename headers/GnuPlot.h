#pragma once

#include "DefTree.h"
#include "my_buffer.h"

const int MAX_GNU_PLOT_FORMULA_LENGTH = 128;
const int MAX_GRAPHIC_NAME_LENGTH     = 128;

#define plot(format, ...) fprintf(gnuplotPipe, format __VA_OPT__(,) __VA_ARGS__)

int MakeGraphic(const char* path, const char* formula);
int DefTreeToGnuFormula (const DefNode *const node, Buffer* buf);