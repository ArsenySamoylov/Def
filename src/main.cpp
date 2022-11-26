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
   $log(RELEASE);
    
   CHECK (UnitTests() == SUCCESS, return LFAILURE);
   
   return LSUCCESS;    
   }              


int NUMBER_O_TESTS = 4;

#pragma GCC diagnostic ignored "-Wunused-variable"
int UnitTests()
    {
    $log(4)
    
    char* sample = GetSrcFile ("UnitTests/sample.txt");
    CHECK (sample, logf("Couldn't open file with test\n"), return LFAILURE);

    size_t num_lines = CountLines(sample);
    $li(num_lines)

    const char** tests = DivideBufferIntoLines (sample, num_lines);

    // SYSTEM("rm -r ./UnitTests/graphics/*.png");
    

    for (size_t i = 0; i < num_lines; i++)
       {
       DefTree def_tree = {};
       static char GraphicName[99] = "";
    
        // $s(tests[i])
        // $s(tests[i+1])

        SYSTEM("rm -rf ./UnitTests/graphics/*.png");
        SYSTEM("rm -rf ./UnitTests/graphics/dot/*.dot");

        SYSTEM("rm -rf ./UnitTests/LaTex/*.log");
        SYSTEM("rm -rf ./UnitTests/LaTex/*.aux");
        SYSTEM("rm -rf ./UnitTests/LaTex/*.tex");
        SYSTEM("rm -rf ./UnitTests/LaTex/*.pdf");

      //////////////////////////////// PARSING
       CHECK (GetG(&def_tree, tests[i]) == SUCCESS, \
                   printf(redcolor "TESTING ENDED WITH ERROR\n" resetconsole), \
                   return LFAILURE);

        sprintf(GraphicName, "%zu__pars", i/2);
        const char* expression = MakeImg(GraphicName, &def_tree);

        int ans = 0;
        sscanf (tests[++i] + 1, "%d\n", &ans);

      //////////////////////////////// DIFFERENTIATE
        DefTree d {};
        d.status = ACTIVE;
        d.root   = Differentiate (def_tree.root, 'x');

       sprintf(GraphicName, "%zu_diff", i/2);
       const char* diff = MakeImg(GraphicName, &d);

      //////////////////////////////// SIMPLIFY
      DefTree simplified{};
      simplified.status = ACTIVE;
      simplified.root   = Simplify (d.root);

      sprintf(GraphicName, "%zu_simple_dif", i/2);
      const char* simple_dif = MakeImg(GraphicName, &simplified);

   
      ///////////////////////////////// LATEX
      static char tex_name[128] = "";
      sprintf (tex_name, "result_%zu", i/2);
      
      CHECK(OpenTexFile (tex_name) == SUCCESS, return LFAILURE);

      char orig_form[MAX_FORMULA_LENGTH] = "";
      Buffer orig_form_buf{};
      BufferCtor (&orig_form_buf, orig_form);

      DefTreeToTex (def_tree.root, &orig_form_buf);
      AddMessage ("Original formula: ");
      AddFormula (orig_form_buf.buffer);
   
      char diff_form[MAX_FORMULA_LENGTH] = "";
      Buffer diff_form_buf {};
      BufferCtor (&diff_form_buf, diff_form);

      DefTreeToTex (d.root, &diff_form_buf);
      AddMessage ("Differentiated formula: ");
      AddFormula (diff_form_buf.buffer);


      char simp_form[MAX_FORMULA_LENGTH] = "";
      Buffer simp_form_buf {};
      BufferCtor (&simp_form_buf, simp_form);

      DefTreeToTex (simplified.root, &simp_form_buf);
      AddMessage ("Simplified differentiated formula: ");
      AddFormula (simp_form_buf.buffer);
         //////////////////////////////// AddGraphics
         Buffer orig_graphic_form{};
         CHECK (BufferCtor(&orig_graphic_form, MAX_GNU_PLOT_FORMULA_LENGTH) == SUCCESS, return LFAILURE);
         CHECK (DefTreeToGnuFormula (def_tree.root, &orig_graphic_form)     == SUCCESS, return LFAILURE);
         
         char graphic_name[MAX_GRAPHIC_NAME_LENGTH] = "";
         sprintf(graphic_name, "UnitTests/graphics/%zugraphicorig.png", i/2); 
         
         MakeGraphic (graphic_name, orig_graphic_form.buffer); 
         AddMessage ("\\begin{center}\n");
         AddMessage ("\\includegraphics [scale = 0.4]{%s}\n", graphic_name); 
         AddMessage ("\\end{center}\n");
   
         //////////////////////////////// Define Variable
         DefineVariable ('x', 3.14, simplified.root);
         DefineVariable ('y', -3, simplified.root);

         sprintf(GraphicName, "%zu_var_dif", i/2);
         const char* var_def = MakeImg(GraphicName, &simplified);

         double result_value = NAN;

         try
         {
            result_value = CountConstants (simplified.root);
         }
         catch (const char* message)
            {
            perror (message);
            
            continue;
            }
         
         AddMessage("\\newline\\newline\n\\\\ \\\\");
         AddMessage("Result, after definnig %c as %lg: \\textbf{%lg}\n", 'x', 3.14, result_value);

      CloseTexFile();

      // SYSTEM("xdg-open %s", simple_dif);
      SYSTEM("open UnitTests/LaTex/result_%zu.pdf", i/2);
      $$

      CHECK (CloseDefTree(&def_tree)   == SUCCESS, return LFAILURE);
      CHECK (CloseDefTree(&d)          == SUCCESS, return LFAILURE);
      CHECK (CloseDefTree(&simplified) == SUCCESS, return LFAILURE);
      }
    
    logf  ("TESTING ENDED SUCCESFULLY\n");
    printf(greencolor "TESTING ENDED SUCCESFULLY\n" resetconsole);
    return LSUCCESS;
    }
