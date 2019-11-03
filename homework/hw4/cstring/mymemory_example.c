#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymemory.h"

int main()
{
        char * mystr = "blair";
        char * copystr = mystrdup(mystr);
        free(copystr);

	char ** arr = malloc(sizeof(char*));
	char ** newarray = add_word(arr, 0, mystr);

	print_word_array(newarray, 1);
	free_word_array(newarray, 1);	
 
        return EXIT_SUCCESS;
}
