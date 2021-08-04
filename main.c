#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "argparser/c/argparser.h"
#include "dictionary.h"

#ifdef _WIN32
    #include<windows.h>
    #define NOOFTHREADS 4
#else
    #include<pthread.h>
    int count = 4;
#endif

#define SIZE 20000

bool obtained = true;
char dictionaryword[50], hint[50];
char description[2048];

void createMatchingWord(char* desc)
{
    obtained = false;
    strncpy(description,desc, sizeof(description));
}

int cDel(char *ptr, char _del)
{
    int count = 0;
    for(int i = 0; i< strlen(ptr); i++)
    {
        if(ptr[i] == _del)
        {
            count++;
        }
    }
    return count;
}

void printbyParts(char* buff)
{
    char * ptr = strtok(buff, ":");
    removeChar(ptr, '"');
    if(strcmp(dictionaryword, ptr)!=0)
    {
        return;
    }
    while(ptr != NULL)
    {
        ptr = strtok(NULL, "\",\n");
        createMatchingWord(ptr); 
        ptr = strtok(NULL, "\",\n");
    }
}

#ifdef _WIN32
    DWORD WINAPI getMeaning()
#else
    void *getMeaning()
#endif
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
    return 0;
}

int main(int argc,char *argv[])
{
    argparser argparse = argparser_create(argc, argv,PARSEMODE_LENIENT);
    argparser_add(&argparse, "-w", "--word", ARGTYPE_STRING,dictionaryword,"Find the meaning of the word");
    // argparser_add(&argparse, "-i", "--hint", ARGTYPE_STRING,hint,"Provide Hint"); 
#ifndef _WIN32
    argparser_add(&argparse, "-c", "--threads", ARGTYPE_INT,&count,"No of Threads Count"); 
#endif

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

#ifdef _WIN32
    HANDLE threads[NOOFTHREADS];
    DWORD threadidArray[NOOFTHREADS];
    for(DWORD i =0; i<NOOFTHREADS; i++)
    {
    threads[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            getMeaning,       // thread function name
            NULL,          // argument to thread function 
            0,                      // use default creation flags 
            &threadidArray[i]);
    }
    WaitForMultipleObjects(NOOFTHREADS, threads,TRUE,INFINITE);
#else
    pthread_t threads[count];
    for(int i = 0; i < count ; i++)
    {
        pthread_create(&threads[i],NULL, getMeaning, NULL);
    }
    for(int i = 0; i < count;i++)
    {
        pthread_join(threads[i],NULL);
    }
    
#endif
    printf("%s :  ",dictionaryword);

    if(strlen(description) != 0) printf("%s \n", description);

    closeFile();
}