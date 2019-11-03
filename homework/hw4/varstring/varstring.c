#include "varstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// We need to define the varstring_t structure which was declared in the
// header before we can use it...
struct varstring_t
{
	char * string;
	unsigned int len;
};

varstring_handle varstring_from(const char * str)
{
	varstring_handle newstring = malloc(sizeof(varstring_handle));
	unsigned int len = strlen(str);
  	newstring->string = malloc(len * sizeof(char*));
  	strcpy(newstring->string, str);

  	return newstring;
}
