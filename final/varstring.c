#include "varstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * mystrdup (const char * s)
{  
   char * n = (char *) malloc (strlen(s)+1);
   strcpy (n, s);
   return n;
}

char * newArray()
{
        char * newArray = malloc(sizeof(1));
        *newArray = '\0';
        return newArray;
}

char * append_char(char * str, char c)
{
        unsigned int len = strlen(str);
        str = realloc(str, sizeof(*str)*(len+2));
        str[len] = c;
        str[len+1] = '\0';
        return str;
}

char ** add_word(char ** array, unsigned int * wordcount, const char * word_to_add)
{
        char * copyword = mystrdup(word_to_add);
        char **newarray = realloc(array, sizeof(*newarray)*(*wordcount+1));
        newarray[*wordcount] = copyword;
        return newarray;
}

const char ** copy_array(char ** oldarray, unsigned int wordcount)
{
        const char ** newarray = malloc(sizeof(*oldarray)*(wordcount));
        for (int h=0; h<wordcount; ++h)
        {
                newarray[h] = oldarray[h];
        }
        return newarray;
}

void free_word_array(char ** array, unsigned int wordcount)
{
        for (unsigned int i=0; i<wordcount; ++i)
        {
                free(array[i]);
        }
        free(array);
}
