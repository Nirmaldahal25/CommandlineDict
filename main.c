#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "argparser/c/argparser.h"
#include "dictionary.h"

#ifdef _WIN32
    #define strtok_r(A,B,C) strtok_s(A,B,C)
#endif

#define SIZE 20000

bool obtained = true;
char dictionaryword[50];
char description[SIZE];

void getMeaning(char* desc)
{
    obtained = false;
    int size = strlen(description);
    strncpy(description + size,desc, SIZE - size);
}

void tokenizeTheLine(char* buff)
{
    char * ptr = strtok_r(buff, ":", &buff);
    removeChar(ptr, '"');
    if(strcmp(dictionaryword, ptr)!=0)
    {
        return;
    }
    while((ptr = strtok_r(buff, ":", &buff)))
    {
        getMeaning(ptr); 
    }
}


void searchDictionary()
{
    char buff[SIZE];
    int size = 0;
    while(obtained){
        fileData(buff, &size);
        if(size < 0) break;
        memset(buff+size,0,SIZE-size);
        tokenizeTheLine(buff);
    }
}

int main(int argc,char *argv[])
{
    argparser argparse = argparser_create(argc, argv,PARSEMODE_LENIENT);
    argparser_add(&argparse, "-w", "--word", ARGTYPE_STRING,dictionaryword,"Find the meaning of the word");
    // argparser_add(&argparse, "-i", "--hint", ARGTYPE_STRING,hint,"Provide Hint"); 

    argparser_parse(&argparse);

    if(strlen(dictionaryword) == 0)
    {
        printf("A word must be inserted. Use help for commands\n");
        exit(EXIT_SUCCESS);
    }
    for(int i=0;i<strnlen(dictionaryword,50);i++)
    {
        dictionaryword[i] = (char)tolower(dictionaryword[i]);
    }
    openFile();
    searchDictionary();
    printf("%s :  ",dictionaryword);

    if(strlen(description) != 0)
    {
        for(int i =0; i<strlen(description); i++ )
        {
            if(description[i] == '\\' && description[i+1]=='n')
            {
                printf("\n");
                i = i + 1;
                continue;
            }
            if(isdigit(description[i]) && description[i+1] == '.')
            {
                printf("\n\n");
            }
            if(description[i]=='\\' && description[i+1] == '"') continue;
            printf("%c",description[i]);
        }
        printf("\n");
    }
    else
    {
        printf("No results Found");
    }

    closeFile();
}