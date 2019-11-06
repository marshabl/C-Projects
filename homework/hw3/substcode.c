#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main (int argc, char ** args)
{
	// if user does not enter 2 or 3 arguments in the command line, end program
        if (argc < 3 || argc > 4)
        {
                puts("Please add correct command line arguments!");
                return EXIT_FAILURE;
        }

	// open files for key and text to encode/decode
	FILE *k = fopen(args[1], "r");
	
	// open file key_used.txt to store encoding used
	FILE *ko = fopen("key_used.txt", "w");

	char c;
	
	// initialize regular alphabet and encoded array alphabet
	const char decode[27] = "abcdefghijklmnopqrstuvwxyz\0";
	char encode[strlen(decode)];

	int i = 0;
	int check = 0;
	
	// generate the key to encode and decode files and store it in file key_used.txt
	while ((c=fgetc(k))!=EOF)
	{
		if (isupper(c))
		{
			c = tolower(c);
		}
		if (isalpha(c) && check == 0 && !strchr(encode, c) && i < 26)
		{
			encode[i] = c;
			fprintf(ko, "%c=%c\n", decode[i], encode[i]); 
			check = 1;
			++i;
		}
		if (c == '.' || c == '?' || c == '!')
		{
			check = 0;
		}
		if (check == 1)
		{
			;
		}
	}

	// if encode array doesn't generate 26 characters, then end program and tell user to use different key text
	if (strlen(encode) < 27)
	{
		puts("Key text doesn't have enough characters a-z! Try a different key text!");
		return EXIT_FAILURE;
	}	

	// if 2 arguments, (key text, and text to be encoded), encode the 3rd argument using key text
	if (argc == 3)
        {
		FILE *e = fopen(args[2], "r");
		int upper = 0;
                
                while ((c=fgetc(e))!=EOF)
		{
			if (isupper(c))
                	{
                        	c = tolower(c);
				upper = 1;
                	}
			if (isalpha(c))
			{
				int i = c - 'a';			
				c = encode[i];
				if (upper == 1)
				{
					c = toupper(c);
					upper = 0;
				}
				putchar(c);
			}
			else 
			{
				putchar(c);
			}
			
		}
		fclose(e);
		return EXIT_SUCCESS;
        }

	// if 3 arguments, (+ extra throwaway argument), decode the 3rd argument
        if (argc == 4)
        {
		FILE *e = fopen(args[2], "r");
		int upper = 0;

		while ((c=fgetc(e))!=EOF)
                {
			if (isupper(c)) 
                        {
                                c = tolower(c);
                                upper = 1;
                        }
                        if (isalpha(c))
                        {
				char *p = strchr(encode, c);
				int i = p - encode;
				//printf("%d", i);
				c = decode[i];
	                        if (upper == 1)
        	                {
                	        	c = toupper(c);
                        	        upper = 0;
                                }
	                        putchar(c);                    
                        }
                        else
                        {
                                putchar(c);
                        }	
		}
		fclose(e);
		return EXIT_SUCCESS;
        }
	
	fclose(k);
	fclose(ko);
	
	return EXIT_SUCCESS;
	
}
