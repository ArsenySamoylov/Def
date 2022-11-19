#ifndef HOW_TO_SPEND_SCHOLARSHIP
#define HOW_TO_SPENF_SCHOLARSHIP

#include "ArsList.h"
#include <stdlib.h>
#include <stdarg.h>

void  CloseHtmlLog ();
FILE* SetHtmlLog   ();

FILE* OpenDot (const char* path);
void  CloseDot ();

extern FILE* HtmlLog;

const int MAX_GRAPH_NAME_LENGTH = 40;
const int     IMAGE_NAME_LENGTH = 40;
const int SYSTEM_COMMAND_LENGTH = 155;

const char* GetImage (ArsList* list);
void SetDot          ();
void SetNodesArr     (ArsList* list);
void SetListStatus   (ArsList* list);
void SetEndDot       ();

void DotPrint     (const char* format, ...);
void ConnectNodes (index_t first, index_t second);

#define set_func_name   fprintf (Dot, "########################################################### %s\n", __func__);

void PrintNode          (node* node_ptr, index_t node_num);
void ConnectPrevAndNext (node* node_ptr, index_t node_num);

void hlog (const char* format, ...);
#endif