#include "SomeStuff.h"

void ShowCharInBinary(char mask)
    {
    char temp = mask;
    int s[8] = {};
    for(int i = 0; i < 8; i++)
        {
        s[i] = temp % 2;
        temp /= 2;
        //printf ("%d", s[i]);
        }

    for (int i =0; i<8;i++)
        printf("%d", s[7-i]);
    
    printf("\n");
    }

int printl (const char *line, char terminator)
    {
    int flag_error = EOF;

    while( *line != '\0' && *line != terminator)
        flag_error = putchar(*(line++));

    flag_error = putchar('\n');

    return flag_error;
    }