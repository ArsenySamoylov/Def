#include "LaTex.h"

#include "Def.h"
#include "Utils.h"
#include "LogMacroses.h"
#include "DSL.h"
#include "my_buffer.h"


static FILE* TEX = NULL;
static char tex_file [MAX_LATEX_NAME_LENGTH + 16] = {};

static void  texprint (const char* format, ...);
static int DefTreeToTex (const DefNode *const node, Buffer* buf);

int OpenTexFile (const char* name)
    {
    if (!name)  return NULL;

    snprintf (tex_file, MAX_LATEX_NAME_LENGTH, "%s%s.tex", 
                                                PATH_TO_TEX, name);

    TEX = fopen (tex_file, "wb"); 
    if (!TEX) return logf("Couldn't create latex file in path %s", tex_file), LFAILURE;

    texprint ("\\documentclass{minimal}\n");
    texprint ("\\usepackage{graphicx}\n");
    texprint ("\\begin{document}\n");
    
    return LSUCCESS; 
    }

void CloseTexFile()
    {
    
    texprint ("\\end {document}\n");
    fflush (TEX);    
   
    SYSTEM ("pdflatex -output-directory=%s %s > output", PATH_TO_PDF, tex_file);
    fclose (TEX);

    TEX = NULL;
    
    return;
    }

int AddDefTreeToTex (const DefNode *const node)
    {
    $log(1)
    assertlog(node, EFAULT, return EFAULT);

    Buffer buf{};
    CHECK (BufferCtor (&buf, MAX_FORMULA_LENGTH) == SUCCESS, return LFAILURE);

    CHECK (DefTreeToTex (node, &buf) == SUCCESS, return LFAILURE);

    texprint("$$");

    texprint(" %s ", buf.buffer);
    texprint("$$\n");

    texprint("\\newline\n");

    KILL((void**) &buf.buffer);

    return SUCCESS;
    }

int DefTreeToTex(const DefNode *const node, Buffer* buf)
    {
    $log(4)

    assertlog (node, EFAULT, return LFAILURE);
    assertlog (buf,  EFAULT, return LFAILURE);

    switch (node->type)
        {
        case CONSTANT: return BufferPutDouble (buf, node->value.t_constant);                     
        case VARIABLE: return BufferPutChar   (buf, node->value.t_variable);
        case OPERATOR:
            { 
            int status = 0; 
            char op = node->value.t_operator;

            status += BufferPutChar (buf, '{');
            status += BufferPutChar (buf, OP(node) == MUL  ? '(' : ' ');
            status += DefTreeToTex  (node->left_child, buf);
            status += BufferPutChar (buf, OP(node) == MUL  ? ')' : ' ');
            status += BufferPutChar (buf, '}');

            if (OP(node) == DIV)
                BufferPutString (buf, " \\over ");
    
            else if (OP(node) == MUL)
                BufferPutString (buf, " \\cdot ");
            else    
                status += BufferPutChar (buf, op);  
            
            status += BufferPutChar (buf, '{');
            status += BufferPutChar (buf, OP(node) == MUL ? '(' : ' ');
            status += DefTreeToTex  (node->right_child, buf);
            status += BufferPutChar (buf, OP(node) == MUL ? ')' : ' ');
            status += BufferPutChar (buf, '}');

            return status;
            }
        case FUNCTOR:  
            {
            int status = 0;
            status += BufferPutString (buf, SUPPORTED_FUNCTIONS[FUNC(node)]);

            status += BufferPutChar (buf, '(');
            status += DefTreeToTex  (node->right_child, buf);
            status += BufferPutChar (buf, ')');

            return status;
            }
        default:
            return logf ("Unknow operator type (%d) for %s", node->type, __FILE__), LFAILURE;
        }

    logf ("You shouldn't reach here %s:%d\n", __func__, __LINE__);
    return  LFAILURE;
    }

int AddMessage (const char* format, ...)
    {
    assertlog(format, EFAULT, return LFAILURE);

    va_list ptr;
    va_start(ptr, format);

    vfprintf (TEX, format, ptr);

    va_end(ptr);

    return SUCCESS;
    } 

static void texprint (const char* format, ...)
    {
    va_list ptr;
    va_start(ptr, format);

    vfprintf (TEX, format, ptr);

    va_end(ptr);
    
    return;
    }

