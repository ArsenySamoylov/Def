#include "Def.h"
#include "ExpressionParser.h"

#include "ArsLib.h"
#include "DefGraphVis.h"

int UnitTests();

int main()
    {
    $log(RELEASE);

    UnitTests();

    return LSUCCESS;    
    }              


int NUMBER_O_TESTS = 4;

#pragma GCC diagnostic ignored "-Wunused-variable"
int UnitTests()
    {
    $log(DEBUG)

    
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

       CHECK (GetG(&def_tree, tests[i]) == SUCCESS, printf(redcolor "TESTING ENDED WITH ERROR\n" resetconsole), return LFAILURE);

        SYSTEM("rm -r ./UnitTests/graphics/*.png");

        sprintf(GraphicName, "%zu__pars", i/2);
        const char* expression = MakeImg(GraphicName, &def_tree);

        int ans = 0;
        sscanf (tests[++i] + 1, "%d\n", &ans);

        DefTree d {};
        d.status = ACTIVE;
        d.root   = Differentiate (def_tree.root);

       sprintf(GraphicName, "%zu_diff", i/2);
       const char* diff = MakeImg(GraphicName, &d);

        // $lp(d.root->right_child->left_child)
       d.root   = Simplify (d.root);
       
    //    $lp(d.root)
       sprintf(GraphicName, "%zu_simple_dif", i/2);
       const char* simple_dif = MakeImg(GraphicName, &d);

       SYSTEM("xdg-open %s", simple_dif);
       $$

       CHECK (CloseDefTree(&def_tree) == SUCCESS, return LFAILURE);
       CHECK (CloseDefTree(&d)        == SUCCESS, return LFAILURE);
       return 0;
       }
    
    logf  ("TESTING ENDED SUCCESFULLY\n");
    printf(greencolor "TESTING ENDED SUCCESFULLY\n" resetconsole);
    return LSUCCESS;
    }
