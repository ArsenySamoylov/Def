#include "Def.h"

#include "ArsLib.h"
#include "DefGraphVis.h"

int main()
    {
    $log(RELEASE);

    DefTree def_tree = {};
    
    CHECK (SetDefTree(&def_tree, "./samples/firstsample.txt") == SUCCESS, return LFAILURE);
    const char* img = MakeImg("sample", &def_tree);
    system("xdg-open ./graphics/sample.png");

    return LSUCCESS;    
    }              