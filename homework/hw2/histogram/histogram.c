#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char ** args)
{
	int digits[10] = {0};
	int letters[26] = {0};
	int i,j;

	FILE *fi = fopen(args[1], "r");

	char c;
	while ((c=fgetc(fi))!=EOF)
	{
		if (c >= 'A' && c<= 'Z')
		{
			++letters[tolower(c)-'a'];
		}
		if (c >= '0' && c<= '9')
		{
			++digits[c-'0'];
		}
		
		if (c >= 'a' && c<= 'z')
		{
			++letters[c-'a'];
		}			
	}
	
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
