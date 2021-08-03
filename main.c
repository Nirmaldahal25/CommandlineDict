#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "argparser/c/argparser.h"
#include "dictionary.h"

#ifdef _WIN32
    #include<windows.h>

    typedef HANDLE threads;
#else
    #include<pthread.h>

    typedef pthread_t threads;
#endif


#define SIZE 20000

bool obtained = true;
char dictionaryword[50], hint[50];
char description[2048];

void createMatchingWord(char* desc)
{
    obtained = false;
    strcpy(description,desc);
}

void printbyParts(char* buff)
{
    char * ptr = strtok(buff, ":");
    removeChar(ptr, '"');
    if(strncmp(dictionaryword, ptr, strlen(dictionaryword))!=0)
    {
        return;
    }
    while(ptr != NULL)
    {
        ptr = strtok(NULL, ":");
        createMatchingWord(ptr); 
        ptr = strtok(NULL, ":");
    }
}

void *getMeaning()
{
    char buff[SIZE];
    int size = 0;
    while(obtained){
        fileData(buff, &size);
        if(size > SIZE)
        {
            printf("size exceeded the buffer");
        }
        if(size == -1){
            obtained = false;
            continue;
        }
        memset(buff+size,0,SIZE-size);
        printbyParts(buff);
        size = 0;
    }
    return NULL;
}

int main(int argc,char *argv[])
{
    argparser argparse = argparser_create(argc, argv,PARSEMODE_LENIENT);
    int count = 4;
    argparser_add(&argparse, "-w", "--word", ARGTYPE_STRING,dictionaryword,"Find the meaning of the word");
    argparser_add(&argparse, "-i", "--hint", ARGTYPE_STRING,hint,"Provide Hint"); 
    argparser_add(&argparse, "-c", "--threads", ARGTYPE_INT,&count,"No of Threads Count"); 
    argparser_parse(&argparse);

    for(int i=0;i<strlen(dictionaryword);i++)
    {
        dictionaryword[i] = tolower(dictionaryword[i]);
    }
    pthread_t threads[count];
    openFile();
    for(int i = 0; i < count ; i++)
    {
        pthread_create(&threads[i],NULL, getMeaning, NULL);
    }
    
    for(int i = 0; i < count;i++)
    {
        pthread_join(threads[i],NULL);
    }
    printf("%s :  ",dictionaryword);

    if(strlen(description) != 0) printf("%s \n", description);

    closeFile();
}