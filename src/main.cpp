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

    SYSTEM("rm -r ./UnitTests/graphics/*");

    for (size_t i = 0; i < num_lines; i++)
       {
       DefTree def_tree = {};

        // $s(tests[i])
        // $s(tests[i+1])

       CHECK (GetG(&def_tree, tests[i]) == SUCCESS, printf(redcolor "TESTING ENDED WITH ERROR\n" resetconsole), return LFAILURE);

        int ans = 0;
        sscanf (tests[++i] + 1, "%d\n", &ans);

       static char GraphicName[99] = "";
    
       sprintf(GraphicName, "./UnitTests/graphics/%zu", i);
       const char* img = MakeImg(GraphicName, &def_tree);

    //    SYSTEM("xdg-open %s", img);
       
       
       CHECK (CloseDefTree(&def_tree) == SUCCESS, return LFAILURE);
       }
    
    logf  ("TESTING ENDED SUCCESFULLY\n");
    printf(greencolor "TESTING ENDED SUCCESFULLY\n" resetconsole);
    return LSUCCESS;
    }
