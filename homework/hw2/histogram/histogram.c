#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char ** args)
{
	//create array of length 10 for digits
	int digits[10] = {0};
	
	//create array of length 26 for letters
	int letters[26] = {0};

	//i, j for 'for' loops
	int i,j;

	//open file given via command line
	FILE *fi = fopen(args[1], "r");

	//read in one character at a time from file
	char c;
	while ((c=fgetc(fi))!=EOF)
	{
		//change upper to lower and count lower
		if (c >= 'A' && c<= 'Z')
		{
			++letters[tolower(c)-'a'];
		}
		//count digits
		if (c >= '0' && c<= '9')
		{
			++digits[c-'0'];
		}
		//count lowers
		if (c >= 'a' && c<= 'z')
		{
			++letters[c-'a'];
		}			
	}
	
	//print histogram of digits
        for (i = 0; i < 10; ++i)
	{
		if(digits[i] > 0)
		{
			printf("%d: ", i);
			for (j=0; j < digits[i]; ++j)
			{
				printf("#");
			}
			printf(" (%d)\n", digits[i]); 
		}
	}
	
	//print histogram of letters
	for (i = 0; i < 26; ++i)
        {
                if(letters[i] > 0)
                {
                        printf("%c: ", (char)(i + 'a'));
                        for (j=0; j < letters[i]; ++j)
                        {
                                printf("#");
                        }
                        printf(" (%d)\n", letters[i]);
                }
        }
	return EXIT_SUCCESS; 
}
