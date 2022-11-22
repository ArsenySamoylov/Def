#include "my_buffer.h"

#include <sys/stat.h>
#include <assert.h>
#include "LogMacroses.h"
#include "CommonEnums.h"

#pragma GCC diagnostic ignored "-Wsign-conversion"

char* GetSrcFile (const char *file_direction)
    {
    if (!file_direction) return NULL;
    
    FILE *txt = fopen(file_direction, "rb");
    if (!txt)
        return NULL;
    
    struct stat txt_data;
    stat(file_direction, &txt_data);

    if (txt_data.st_size < 1)
        {
        fclose(txt);
        return NULL;
        }

    char* txt_buffer = (char*) calloc (txt_data.st_size + 1, sizeof (char));
    if (!txt_buffer)
        return NULL;
  
    size_t number_of_ch = fread (txt_buffer, sizeof(char), txt_data.st_size, txt);

    char* temp = (char*)realloc(txt_buffer, number_of_ch + 1);
    if   (temp != NULL)
        txt_buffer = temp;
        
    txt_buffer[number_of_ch] = '\0';

    fclose(txt);

    return txt_buffer;
    }

//! @note This functions changes all '\n' to '\0' while dividing buffer 
//! If you want to split buffer without changing '\n', then use SplitBufferIntoLines
const char** DivideBufferIntoLines (char* buffer, size_t number_of_lines)
    {
    if (!buffer)          return NULL;
    if (!number_of_lines) return NULL;

    const char** lines_array = (const char**) calloc (number_of_lines + 1, sizeof(lines_array[0]));
    if (lines_array == NULL) return NULL;

    size_t current_line = 0;
    int number_of_characters_in_line = 0;
    size_t i = 0;

    while (buffer[i] != '\0')
        {
        if (buffer[i] == '\r')
            buffer[i] = ' ';

        if (buffer[i] == '\n')
            {
            buffer[i] = '\0';
            lines_array[current_line] = (buffer + i - number_of_characters_in_line);

            number_of_characters_in_line = 0;
            current_line++;
            }

        else if (buffer[i+1] == '\0')
            {
            lines_array[current_line] = (buffer + i - number_of_characters_in_line);

            current_line++;
            number_of_characters_in_line = 0;
            }

        else
            number_of_characters_in_line++;

        i++;
        }

    lines_array[number_of_lines] = NULL;

    return lines_array;
    }

const char** SplitBufferIntoLines (const char* buffer, size_t number_of_lines)
    {
    if (!buffer)          return NULL;
    if (!number_of_lines) return NULL;

    const char** lines_array = (const char**) calloc (number_of_lines + 1, sizeof(lines_array[0]));
    if (lines_array == NULL) return NULL;

    size_t current_line = 0;
    int number_of_characters_in_line = 0;
    size_t i = 0;

    while (buffer[i] != '\0')
        {
        if (buffer[i] == '\n')
            {
            lines_array[current_line] = (buffer + i - number_of_characters_in_line);

            number_of_characters_in_line = 0;
            current_line++;
            }

        else if (buffer[i+1] == '\0')
            {
            lines_array[current_line] = (buffer + i - number_of_characters_in_line);

            current_line++;
            number_of_characters_in_line = 0;
            }

        else
            number_of_characters_in_line++;

        i++;
        }

    lines_array[number_of_lines] = NULL;

    return lines_array;
    }

size_t CountLines(const char* text, char end_of_line)
    {
    if (!text)
        return 0;

    size_t number_of_lines = 0;
    size_t i = 0;

    while (text[i] != '\0')
        {
        if (text[i] == end_of_line || text[i+1] == '\0' )
            number_of_lines++;

        i++;
        }

    return number_of_lines;
    }

void RemoveComments (char* buffer, char terminator)
    {
    if (!buffer) return;
    
    while (*buffer != '\0')
        {
        if (*buffer == terminator)
            *buffer = '\0';

        ++buffer;    
        }
    
    return;
    }
    
void RemoveComments (char* buffer, unsigned size, char terminator)
    {
    if (!buffer)
        return;
    
    unsigned i = 0;
    while (i < size)
        {
        if (*(buffer + i) == terminator)
            *(buffer + i) = '\0';
            
        ++i;
        }
    
    return;
    }

#pragma GCC diagnostic ignored "-Wcast-qual"
#include <ctype.h>
char* SkipSpaces(const char* s)
    {
    if (!s)
        return NULL;
    
    int n = 0;
    sscanf(s,"%*[ \t\n\v]%n", &n);

    return (char*)(s + n);
    }

void ShowLine(const char* line)
    {
    //printf("Line is (%s) ", line);
   
    while(*line != '\0')
        {
        if (*line == ' ')
            putchar('_');
        else
        if (*line == '\n')
            printf("\\n");
        else
        if (*line == '\t')
            printf("\\t");
        else
        if (*line == '\b')
            printf("\\b");
        else
        if (*line == '\r')
            printf("\\r");
        else
            putchar( *line);
        
        line++;
        }
    
    printf("\\0\n");
    }

int stricmp (const char *s1, const char *s2 )
    {
    int i = 0;

    for( ; s1[i] && s2[i] ; i++)
        if ((tolower(s1[i]) != tolower(s2[i])))
            return s1[i] - s2[i];   
        
    return (s1[i] - s2[i]);
    }

int strnicmp (const char *s1, const char *s2, int number_of_ch)
    {
    int i = 0;

    for( ; s1[i] && s2[i] && number_of_ch; i++, number_of_ch--)
        if ((tolower(s1[i]) != tolower(s2[i])))
            return s1[i] - s2[i];   
        
    return (s1[i] - s2[i]);
    }

#pragma GCC diagnostic ignored "-Wmultichar"
int strnicmprus (const char *s1, const char *s2, int number_of_ch)
    {
    int i = 0;

    // #include "EasyDebug.h"
    // $i(s1[1] - s2[1])  
    // //$i('а'-'А')
    // $i(s1[1])
    // $i(s2[1])
    // printf("\n");  
    
    for( ; s1[i] && s2[i] && number_of_ch; i++, number_of_ch--)
        if (s1[i] != s2[i] && abs(s1[i] - s2[i]) != 32) // change to 'а' - 'А' 
            return s1[i] - s2[i];   
    //$
    if (abs (s1[i] - s2[i]) == 32)
        return 0;

    return s1[i] - s2[i]; // cause they don't cast to lower caase hence differrence ain't 0
    }

void ClearBuffer ()
    {
    while (getchar() != '\n')
        continue;

    return;  
    }

void fsetindent (FILE* file, int indent)
    {
    fprintf(file, "%*s", indent * INDENT_SIZE, "");

    return;
    }

void setindent (int indent)
    {
    printf("%*s", indent * INDENT_SIZE, "");

    return;
    }

int BufferCtor (Buffer* buf, const char* string)
    {
    $log(DEBUG)
    CHECK (buf, return LFAILURE);

    buf->buffer = buf->str = string;

    return LSUCCESS;
    }

void BufferUngetCh (Buffer* buf)
    {
    assert(buf);

    if (buf->str > buf->buffer)
        (buf->str)--;

    return;
    }

int BufferGetCh (Buffer* buf)
    {
    assert(buf);

    int n = 0;
    sscanf(buf->str, "%*[ \t\n\v]%n", &n);

    // $li(n)
    // $lc(*buf->str)
    // $ls(buf->str)
    buf->str += n;

    return *(buf->str++);
    }

int BufferLook (Buffer* buf)
    {
    assert(buf);

    int n = 0;
    sscanf(buf->str, "%*[ \t\n\v]%n", &n);

    // $li(n)
    // $lc(*buf->str)
    // $ls(buf->str)

    return *(buf->str + n);
    }

int  BufferGetDouble (Buffer* buf, double* val)
    {
    assert(buf);

    int n   = 0;

    // logf("before: %s\n", buf->str);

    if (sscanf(buf->str, "%lg%n", val, &n) == 0)
        return FAILURE;

    buf->str += n;
    // logf("after: %s\n", buf->str);

    return SUCCESS;
    }