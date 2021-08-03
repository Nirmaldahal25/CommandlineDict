#ifndef DICTIONARY
#define DICTIONARY 1

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

    // typedef struct Results
    // {
    //     char *word;
    //     char *description;
    //     int size;
    //     struct Results* next;
    // } Results;

    //open the dictionary file
    void openFile();

    //Attach to previous structure
    void fileData(char buff[], int* size);
    
    //remove a character from the array
    void removeChar(char str[],char t);

    //close the file pointer
    void closeFile();
#endif