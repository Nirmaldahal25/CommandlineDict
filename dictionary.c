#include "dictionary.h"

FILE* filepointer;
int filesize;
const char* filename = "dictionary.json";


#ifdef _WIN32
size_t getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*lineptr == NULL) {
        *lineptr = malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}
#endif

void openFile()
{

#ifdef _WIN32
    if ((filepointer = _fsopen (filename, "r",_SH_DENYRW)) < 0)
#else
    if ((filepointer = fopen (filename, "r")) < 0)
#endif
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

#ifdef _WIN32
    *size = getline(&line, &len, filepointer );
#else
    *size = getline(&line, &len, filepointer);
#endif
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