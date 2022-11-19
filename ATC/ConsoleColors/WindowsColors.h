#ifndef COLORS_H
#define COLORS_H

#include <windows.h>

//#include <stdio.h>

enum myCOLORS
    {
    black,
    blue,
    green,
    cyan,
    red,
    lil,
    yelow,
    st_cl,
    gray,
    lt_blue,
    lt_green,
    lt_cyan,
    lt_red,
    lt_yellow,
    //?
    lt_white = 15,
    };

//??? wtf ded ???
unsigned txGetConsoleAttr(); //tx_nodiscard;

int SetColor(unsigned back_ground, unsigned fore_ground )
    {
    unsigned old_attr = txGetConsoleAttr();

    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), (back_ground<<4) | fore_ground);

    return old_attr;    
    }
// thx to DED for TXlib    
unsigned txGetConsoleAttr()
    {
    CONSOLE_SCREEN_BUFFER_INFO con = {};
    GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &con);

    return con.wAttributes;
    }

class RestoreOldAttr
    {
    private:
        unsigned old_attr;
    public:
        RestoreOldAttr(unsigned attr)
            {
            old_attr = attr;
            }  
    
        ~RestoreOldAttr()
            {
            // printf("GREAT DESTRUCTOR, oldatrr = %d\n", old_attr);
            SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), old_attr);
            }
    };

#endif
