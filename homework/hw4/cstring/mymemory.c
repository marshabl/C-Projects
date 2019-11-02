#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define mystrdup
// determine length of the string
// allocate enough memory to hold a copy of that string including null terminator
// copy contents of string to new allocated memory
// return a pointer to the new string

char * mystrdup(const char * mystr)
{
	unsigned int len = strlen(mystr);
	char* ptr = malloc((len+1)*sizeof(mystr));
	for (unsigned int i=0; i<len; ++i)
	{
		ptr[i] = mystr[i];
	}
	ptr[len] = '\0';	
	return ptr;
}

char ** add_word(char ** array, unsigned int wordcount, const char * word_to_add)
{
	char * copyword = mystrdup(word_to_add);

	unsigned int len = sizeof(array) / sizeof(char);
	
	wordcount = 0;
	for (unsigned int i=0; i < len; ++i)
	{
		wordcount = i;
	}
	
	char ** newarray = malloc((wordcount+1)*sizeof(array));
	for (unsigned int i=0; i < wordcount; ++i)
	{
		newarray[i] = array[i];	
	}

        newarray[wordcount] = copyword;
        //newarray[wordcount + 1] = '\0';

	return newarray;
}
