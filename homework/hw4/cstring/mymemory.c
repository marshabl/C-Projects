#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * mystrdup(const char * mystr)
{
	unsigned int len = strlen(mystr); // determine length of the string
	char* ptr = malloc((len+1)*sizeof(char)); // alloc mem to hold a copy incl. null term
	strcpy(ptr, mystr); // copy string to new alloc memory
	return ptr;
}

char ** add_word(char ** array, unsigned int wordcount, const char * word_to_add)
{
	char * copyword = mystrdup(word_to_add); // create copy of word to add
	char **newarray = realloc(array, sizeof(char)*(wordcount+1)); // add one to size of new array
	newarray[wordcount] = copyword; // copy word to new array
	return newarray;
}

void free_word_array(char ** array, unsigned int wordcount)
{
	for (unsigned int i=0; i<wordcount; ++i)
	{
		free(array[i]);
	}
}

void print_word_array(char ** array, unsigned int wordcount)
{
	for (unsigned int i=0; i<wordcount; ++i)
        {
                printf("%s\n", array[i]);
        }
}
