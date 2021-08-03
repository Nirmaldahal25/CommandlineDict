#include "dictionary.h"

FILE* filepointer;
int filesize;
const char* filename = "dictionary.json";

void openFile()
{
    if ((filepointer = fopen (filename, "r")) < 0)
    {
        fprintf(stderr, "cannot open file '%s' for reading:\n",filename);
        return;
    }
    fseek(filepointer, 0, SEEK_END);
    filesize = ftell(filepointer);
    rewind(filepointer);
}

void closeFile()
{
    fclose(filepointer);
}

void fileData(char buff[], int *size)
{
    size_t len = 0;
    char* line = NULL;

    flockfile(filepointer);
    *size = getline(&line, &len, filepointer );
    funlockfile(filepointer);

    if(*size != -1) 
    {
        strncpy(buff,line,*size);
        free(line);
    }
}



void removeChar(char* str,char t)
{
    int i,j;
    i = 0;
    while(i<strlen(str))
    {
        if (str[i]==t || str[i]==' ') 
        { 
            for (j=i; j<strlen(str); j++)
                str[j]=str[j+1];   
        } 
        else i++;
    }
}