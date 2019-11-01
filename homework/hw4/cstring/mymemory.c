#include "mymemory.h"
#include <stdlib.h>
#include <string.h>

// define mystrdup
// determine length of the string
// allocate enough memory to hold a copy of that string including null terminator
// copy contents of string to new allocated memory
// return a pointer to the new string

char * mystrdup(const char * mystr)
{
	unsigned int length  = strlen(mystr);
	char* ptr = malloc(sizeof(mystr));
	for (unsigned int i=0; i<length; ++i)
	{
		ptr[i] = mystr[i];
	}
	ptr[length] = '\0';	
	return ptr;
}
