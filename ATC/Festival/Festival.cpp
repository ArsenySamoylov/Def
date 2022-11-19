#include "Festival.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void festsay (const char* text)
    {
    assert(text);

    static char FestCMD [FESTIVAL_COMMAND_LENGHT]= "";
    memset (FestCMD, 0,  FESTIVAL_COMMAND_LENGHT);

    sprintf (FestCMD, FESTIVAL_ZALUPA, text);

    // $s(FESTIVAL_GREETING)
    // $s(FESTIVAL_ZALUPA)
    // $s(GREETING)
    // $$

    system (FestCMD);

    return;
    }