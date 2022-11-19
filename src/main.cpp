#include "Def.h"

#include "ArsLib.h"

int main()
    {
    $log(RELEASE);

    DefTree def_tree = {};
    
    CHECK (SetDefTree(&def_tree, "./samples/firstsample.txt") == SUCCESS, return LFAILURE);
      
    return LSUCCESS;    
    }              