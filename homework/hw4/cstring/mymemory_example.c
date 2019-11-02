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

	char * myarr[3] = {"Blair", "was", "here"};
	unsigned int wordcount;

	char ** newarray = add_word(myarr, wordcount, mystr);

	printf("%s\n", newarray[3]);
	free(newarray);	
 
        return EXIT_SUCCESS;
}
