#include "Def.h"

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

int UnitTests()
    {
    $log(DEBUG)

    DefTree def_tree = {};
    

    SYSTEM("rm -r ./UnitTests/graphics/*");

    for (int i = 0; i <NUMBER_O_TESTS; i++)
       {
       static char FileName[99] = "";
       sprintf(FileName, "./UnitTests/%d", i);
       
       CHECK (SetDefTree(&def_tree, FileName) == SUCCESS, return LFAILURE);

       static char GraphicName[99] = "";
    
       sprintf(GraphicName, "./UnitTests/graphics/%d", i);
       const char* img = MakeImg(GraphicName, &def_tree);

    //    SYSTEM("xdg-open %s", img);
       
       CHECK (CloseDefTree(&def_tree) == SUCCESS, return LFAILURE);
       }

    return 0;
    }
