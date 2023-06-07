#include "Def.h"

#include <stddef.h>
#include <math.h>

#include "ExpressionParser.h"

#include "ArsLib.h"
#include "DefGraphVis.h"

#include "LaTex.h"
#include "GnuPlot.h"

int UnitTests();

int main()
   {    
   $log(RELEASE)
    
   CHECK (UnitTests() == SUCCESS, return LFAILURE);
   
   return LSUCCESS;    
   }              


int NUMBER_O_TESTS = 1;

#pragma GCC diagnostic ignored "-Wunused-variable"
int UnitTests()
    {
    $log(4)
    
    char* sample = GetSrcFile ("UnitTests/sample.txt");
    CHECK (sample, logf("Couldn't open file with test\n"), return LFAILURE);

    size_t num_lines = CountLines(sample);
    $li(num_lines)

    const char** tests = DivideBufferIntoLines (sample, num_lines);

    for (size_t i = 0; i < num_lines; i += 2)
       {
       static char GraphicName[99] = "";
    
        SYSTEM("rm -rf ./UnitTests/graphics/*.png");
        SYSTEM("rm -rf ./UnitTests/graphics/dot/*.dot");

        SYSTEM("rm -rf ./UnitTests/LaTex/*.log");
        SYSTEM("rm -rf ./UnitTests/LaTex/*.aux");
        SYSTEM("rm -rf ./UnitTests/LaTex/*.tex");
        SYSTEM("rm -rf ./UnitTests/LaTex/*.pdf");

      //////////////////////////////// PARSING
      Token* token_arr = nullptr;
      int number_of_tokens = Tokenizer (&token_arr, tests[i]);

      DefNode* root = GetG (token_arr, number_of_tokens);
      if (!root)
         {
         printf(redcolor "TESTING ENDED WITH ERROR\n" resetconsole);
         return LFAILURE;
         }

      sprintf(GraphicName, "%zu__pars", i/2);
      const char* expression = MakeImg(GraphicName, root);

      //////////////////////////////// DIFFERENTIATE
      DefNode* diff = Differentiate (root, 'x');

       sprintf(GraphicName, "%zu_diff", i/2);
       const char* diff_png = MakeImg(GraphicName, diff);

      //////////////////////////////// SIMPLIFY
      diff = Simplify (diff);

      sprintf(GraphicName, "%zu_simple_dif", i/2);
      const char* simple_dif = MakeImg(GraphicName, diff);
            
      ///////////////////////////////// LATEX
      static char tex_name[128] = "";
      sprintf (tex_name, "result_%zu", i/2);
      
      CHECK(OpenTexFile (tex_name) == SUCCESS, return LFAILURE);

      AddMessage ("Original formula: ");
      AddDefTreeToTex (root);
   
      AddMessage ("Differentiated formula (simplified): ");
      AddDefTreeToTex (diff);

      //////////////////////////////// AddGraphics
      Buffer orig_graphic_form{};
      CHECK (BufferCtor(&orig_graphic_form, MAX_GNU_PLOT_FORMULA_LENGTH) == SUCCESS, return LFAILURE);
      CHECK (DefTreeToGnuFormula (diff, &orig_graphic_form)     == SUCCESS, return LFAILURE);
      
      char graphic_name[MAX_GRAPHIC_NAME_LENGTH] = "";
      sprintf(graphic_name, "UnitTests/graphics/%zugraphicorig.png", i/2); 
      
      MakeGraphic (graphic_name, orig_graphic_form.buffer); 
      AddMessage ("\\begin{center}\n");
      AddMessage ("\\includegraphics [scale = 0.4]{%s}\n", graphic_name); 
      AddMessage ("\\end{center}\n");
   
      //////////////////////////////// Define Variable
      char var = 0;
      double val = NAN;
      int number_of_mem = 0;

      sscanf(tests[i+1], "#%c=%lg, n=%i", &var, &val, &number_of_mem);

      DefineVariable (var, val, diff);

      sprintf(GraphicName, "%zu_var_dif", i/2);
      const char* var_def = MakeImg(GraphicName, diff);

      double result_value = NAN;

      try
      {
         result_value = CountConstants (diff);
      }
      catch (const char* message)
         {
         perror (message);
         
         continue;
         }
         
      AddMessage("\\newline\\newline\n\\\\ \\\\");
      AddMessage("Result, after definnig %c as %lg: \\textbf{%lg}\n\n\n", 'x', val, result_value);

   AddMessage ("Teylor series (%d members): \n", number_of_mem);
   Teylor (root, number_of_mem, var, val); 
   CloseTexFile();

   KILL((void**) &token_arr);
   DeleteBranch (diff);
   }
    

    logf  ("TESTING ENDED SUCCESFULLY\n");
    printf(greencolor "TESTING ENDED SUCCESFULLY\n" resetconsole);
    return LSUCCESS;
    }
   
