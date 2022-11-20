#include "Def.h"

#include "ArsLib.h"
#include "DefGraphVis.h"

int UnitTests(DefTree* def_tree);

int main()
    {
    $log(RELEASE);

    DefTree def_tree = {};
    
    UnitTests(&def_tree);

    return LSUCCESS;    
    }              



int NUMBER_O_TESTS = 4;

int UnitTests(DefTree* def_tree)
    {
    $log(DEBUG)
    assert(def_tree);

    for (int i = 0; i <NUMBER_O_TESTS; i++)
       {
       char FileName[99] = "";
       sprintf(FileName, "./UnitTests/%d", i);
       
       CHECK (SetDefTree(def_tree, FileName) == SUCCESS, return LFAILURE);

       char GraphicName[99] = "";
       sprintf(GraphicName, "./UnitTests/graphics/%d", i);
       const char* img = MakeImg(GraphicName, def_tree);

       SYSTEM("xdg-open %s", img);
       }

    return 0;
    }
