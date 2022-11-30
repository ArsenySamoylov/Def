#include "GnuPlot.h"

#include <stdio.h>
#include <unistd.h>

#include "LogMacroses.h"
#include "DSL.h"
#include "Def.h"

int MakeGraphic(const char* path, const char* formula)
      {
      $log(0)
      assertlog(path, EFAULT, return LFAILURE);
      
      FILE* gnuplotPipe = popen("gnuplot -persistent", "w");

      plot("set terminal png size 800, 600\n");
      plot("set output \"%s\" \n", path);
      
      plot("set title \"Test\"\n");
      
      plot("set xrange  [ * : * ] noreverse writeback\n");
      plot("set x2range [ * : * ] noreverse writeback\n");
      plot("set yrange  [ * : * ] noreverse writeback\n");
      plot("set y2range [ * : * ] noreverse writeback\n");
      plot("set zrange  [ * : * ] noreverse writeback\n");
      plot("set cbrange [ * : * ] noreverse writeback\n");
      plot("set rrange  [ * : * ] noreverse writeback\n");
      // plot("set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front  noinvert bdefault");
      plot("NO_ANIMATION = 1\n");
      plot("plot [-10:10] %s\n", formula);

    //   plot("e");
      pclose(gnuplotPipe);
      
      return SUCCESS;
      }

int DefTreeToGnuFormula (const DefNode *const node, Buffer* buf)
    {
    $log(4)

    assertlog (node, EFAULT, return LFAILURE);
    assertlog (buf,  EFAULT, return LFAILURE);

    switch (node->type)
        {
        case CONSTANT: return BufferPutDouble (buf, CONST(node));                     
        case VARIABLE: return BufferPutChar   (buf,  VAR(node));
        case OPERATOR:
            { 
            int status = 0; 

            status += BufferPutChar (buf, '(');
            status += DefTreeToGnuFormula  (LEFT(node), buf);
            status += BufferPutChar (buf, ')');

            if (OP(node) == POW)
                BufferPutString (buf, "**");
            else    
                status += BufferPutChar (buf, OP(node));  
            
            status += BufferPutChar (buf, '(');
            status += DefTreeToGnuFormula  (RIGHT(node), buf);
            status += BufferPutChar (buf, ')');

            return status;
            }
        case FUNCTOR:  
            {
            int status = 0;
            status += BufferPutString (buf, SUPPORTED_FUNCTIONS[FUNC(node)]);

            status += BufferPutChar (buf, '(');
            status += DefTreeToGnuFormula (RIGHT(node), buf);
            status += BufferPutChar (buf, ')');

            return status;
            }
        default:
            return logf ("Unknow operator type (%d) for %s", node->type, __FILE__), LFAILURE;
        }

    logf ("You shouldn't reach here %s:%d\n", __func__, __LINE__);
    return  LFAILURE;
    }