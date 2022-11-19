#include "DataBase.h"

#include <time.h>
#include "ArsLib.h"

static void LogNode (const Node* node);

static Node* SetNewNode    (Node* parent,       const char** buffer);
static Node* FinishSetting (Node* current_node, const char** buffer);
static Node* ParsingErrorMessage (const char* message, const char* buffer); // return value now is uselles


int OpenDataBase (DataBase* data_base, const char* path)
    {
    assertlog (data_base, EFAULT, exit(FAILURE));
    assertlog (path,      ENOENT, exit(FAILURE)); // MB in this case open new data base ??? 

    const char* buffer = GetSrcFile(path);
    assertlog (buffer, ENOENT, \
          return  MsgRet (FAILURE, "Can't find data base in path: %s\n", path)); //  MsgNoRet ("TO do: in this case create std empty data base\n") 


    BinaryTree* data_tree = (BinaryTree*) calloc (1, sizeof(data_tree[0]));
    assertlog (data_tree, ENOMEM, return FAILURE);

    Ctor(data_tree);

    int status = BufferToTreeDataBase (data_tree, buffer);
    if (status != SUCCESS)
        return LogMsgRet (FAILURE, "Error occured while opening data base\n");

    data_base->buffer    = buffer;
    data_base->data_tree = data_tree;

    return LogMsgRet (SUCCESS, "Data base setted succesfuly (from path: %s)\n\n", path);
    }

int UpdateDataBase (DataBase* data_base)
    {
    assert(data_base);

    // backup
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    char backup_name[56] = "";
    sprintf   (backup_name, "DataBase//BackUp//base_from_%d_%d_%d(%dh%dmin).ak", 
                             timeinfo->tm_mon, timeinfo->tm_mday, 
                             timeinfo->tm_year + 1900, timeinfo->tm_hour, 
                             timeinfo->tm_min);
    
    FILE* old_base = fopen (backup_name, "w");
    assertlog (old_base, EBADF, return LogMsgRet (FAILURE, "Could't create file for backup\n"));
    
    fprintf (old_base, "%s", data_base->buffer);
    fclose  (old_base);

    // set new base
    FILE* new_base = fopen ("DataBase/sample.ak", "w");
    
    SetNewDataBase (new_base, data_base->data_tree->root);
    fclose(new_base);

    return SUCCESS;
    }

//#define setoffset(num) int a = num; while (a-- > 0) {fprintf(new_base, "\t");}


int SetNewDataBase (FILE* new_base, Node* node)
    {
    assert(new_base);

    static int offset = 0;

    fprintf(new_base, "\n");
    fsetoffset (new_base, offset++);
    fprintf(new_base, "{ \"%s\"", node->data);
    
    //fsetoffset (new_base, offset);

    if (node->first_child)
        SetNewDataBase(new_base, node->first_child);

    if (node->second_child)
        SetNewDataBase(new_base, node->second_child);

    //offset--;
    //fsetoffset (new_base, offset);
    fprintf(new_base, "}\n");
    
    return SUCCESS;
    }

int BufferToTreeDataBase (BinaryTree* data_tree, const char* buffer)
    {
    assertlog (data_tree, EFAULT, exit(FAILURE));
    assertlog (buffer,    EINVAL, exit(FAILURE));

    // check that data_tree is empty
    if (data_tree->size)
        return MsgRet (FAILURE, "DataTree must be empty\n");

    // setting everything to start parsing
    buffer = SkipSpaces(buffer);

    Node* current_node = NULL;
    int number_of_nodes = 0;

    // parsing
    while (*buffer)
        {
        if (*buffer == '{') // @todo to functions
            {
            Node* new_node = SetNewNode (current_node, &buffer);
            if (!new_node)
                {
                printf("Here error occured @todo, check this place\n");
                // error
                return SYNTAX_ERROR;
                }

            current_node = new_node;
            number_of_nodes++;
            }

        if (*buffer == '}')
            {
            Node* prev_node = FinishSetting(current_node, &buffer);
            if (!prev_node)
                {
                printf("Here error occured @todo, check this place\n");    
                // error
                return SYNTAX_ERROR;
                }

            current_node = prev_node;
            continue;
            }
        
        
        if (*buffer != '}' && *buffer != '{')
            {
            printf ("Unknown character %c\n In line: ", *buffer);
            printl ( buffer, '\n');

            return SYNTAX_ERROR; 
            }
        }
    
    // setting everything after parsing

    // check that current_node is root
    log ("\nROOT:::::::::::::::::::::::::::::\n");
    LogNode (current_node);
    log (  ":::::::::::::::::::::::::::::::::\n\n");

    if (current_node->parent)
        return LogMsgRet (FAILURE, "Erro occurred, current_node must have NULL parent\n");

    data_tree->root = current_node;
    data_tree->size = number_of_nodes;

    return SUCCESS;
    }


static Node* ParsingErrorMessage (const char* message, const char* buffer)
    {   
    printf ("Error in line : ");
    printl (buffer, '\n');
    printf ("%s", message);

    return NULL;
    }


static void LogNode (const Node* node)
    {
    log ("\nparent: %p\nData: (%s)\nAddress: %p\n\tFChild: %p \t Schild: %p\n", node->parent,
                node->data, node, node->first_child, node->second_child);

    return;
    }       

static Node* SetNewNode (Node* parent, const char** buffer)
    {
    //assert(parent); // PARENT CAN BE NULL !!!    
    assert(buffer);

    Node* new_node = (Node*) calloc (1, sizeof(new_node[0]));
    CHECK_STDERR(new_node, return NULL);

    new_node->parent = parent;

    *buffer = SkipSpaces (++(*buffer));       // should I free new_node in case of error ??? 

    if (**buffer != '"')
        return ParsingErrorMessage ("After { (opening braket) must be \"statement\" in this brakets - \"\n", *buffer);

    int n = 0;

    int status = sscanf (++(*buffer), "%[^\"]%n", new_node->data, &n);

    if (!status)
        return ParsingErrorMessage ("sscanf could't read this line\n", *buffer);

    if (!n)
        return ParsingErrorMessage ("\"statement\" is empty\n", *buffer);

    if (*(*buffer + n) != '"')
        return ParsingErrorMessage ("\"statement\" missing closig \" bracket \n", *buffer);

    *buffer = SkipSpaces(*buffer + n + 1);

    new_node->first_child  = NULL;
    new_node->second_child = NULL;

    // current_node = new_node;
    // number_of_nodes++;

    LogNode (new_node);

    return new_node;
    }


static Node* FinishSetting (Node* current_node, const char** buffer)
    {
    assert(current_node);
    assert(buffer);

    Node* prev_node = current_node->parent;

    // if prev_node is NULL, than current_node is root
    if (!prev_node)
        {
        *buffer = SkipSpaces(++(*buffer));   

        return current_node;
        }

    if (!prev_node->first_child)
            prev_node->first_child = current_node;

    else 

        if (!prev_node->second_child)
                prev_node->second_child = current_node;

        else
            {
            printf ("Error in line : ");
            printl ( *buffer, '\n');
            printf ("More specificly: %.20s\n", *buffer);

            printf ("Data tree node can't have more then two children\n");

            return NULL;
            }
        
    // (prev_node->first_child == NULL) ? prev_node->first_child : prev_node->second_child = current_node;
    LogNode (prev_node);

    current_node = prev_node;
    
    *buffer = SkipSpaces(++(*buffer));

    return prev_node;
    }