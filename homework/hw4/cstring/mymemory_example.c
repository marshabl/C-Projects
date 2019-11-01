#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymemory.h"

int main()
{
        char * mystr = "blair";
        char * copystr = mystrdup(mystr);

        printf("%s\n", copystr);
        free(copystr);

	char ** wordarray;
	unsigned int wordcount;
	char ** newarray = add_word(wordarray, wordcount, mystr);

	printf("%s\n", newarray[0]);
	free(copystr);	
 
        return EXIT_SUCCESS;
}
