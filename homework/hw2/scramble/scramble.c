#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// function to scramble words
//PLEASE NOTE, I WAS UNABLE TO FIGURE OUT HOW TO MAKE THEM A UNIQUE SET
int scramble(char j[128])
{
	//initialize strlen and empty string
	int l = strlen(j);
	char y[128];
	
	//initialize pseudorandom with srand
	srand(time(0));

	//loop through characters of string and assign each one a new random value
	for(int i=0; i<l; ++i)
	{	
		//rand to get rand value and modulo l to keep it in strlen range
		int q = rand() % l;
		
		//give empty string new random values
		y[i] = j[q];
	}
	
	printf("%.*s\n", l, y);
	return EXIT_SUCCESS;
}

int main(int argc, char ** args)
{
	//open file given via command line
        FILE *fi = fopen(args[1], "r");
	
	if (fi == NULL)
   	{
      		// print error if no file and exit program
      		perror("There was a problem opening the file: ");
      		return EXIT_FAILURE;
   	}

		
        //read in one line at a time
        char s[1024];
        while ((fgets(s, sizeof s, fi))!=NULL)
	{
		//keep track of new words in matrix
		char string[1024][128];
		int count = 0;
		int k = 0;

		//loop through line and store words
		for(int r=0; r<strlen(s); ++r)
		{
			//check if blank, non-alphanumerica, or end of line
			if (!isblank(s[r]) && isalnum(s[r]) && s[r]!= '\n')
      			{
				string[k][count] = s[r];
			        ++count;
			}
			
			//if blank or end of line scramble word and print to console
			if (isblank(s[r]) || s[r]=='\n')
      			{
				scramble(string[k]);
			        ++k;
			        count = 0;
			}
		}
	}

	fclose(fi);
	return EXIT_SUCCESS;
}
