#include "varstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
	if (argc == 1)
	{
		varstring_handle b = varstring_new();
        	varstring_handle s = varstring_from("my first varstring");
        	varstring_append_str(s, " was very easy to create!");
        	varstring_append_str(b, "Hello, world");
        	varstring_set_char(b, 0, 'B');
        	puts(varstring_access(s));
        	puts(varstring_access(b));
        	int pos = varstring_get_char(b, 5);
        	printf("%d\n", pos);

        	varstring_handle c = varstring_from("Blair was here");
        	varstring_append_char(c, '!');

        	char * realc = varstring_convert(c);
        	printf("%s\n", realc);

        	varstring_append_varstring(c, b);
        	puts(varstring_access(c));

        	varstring_handle j = varstring_from("Joker");
        	varstring_insert_str(j, 1,  "Batman");
        	puts(varstring_access(j));
        	varstring_free(j);

        	varstring_free(c);
        	varstring_free(b);
        	varstring_free(s);

        	return EXIT_SUCCESS;
	}
	else
	{
		varstring_handle b = varstring_new();
		for (int i=1; i<argc; ++i)
		{
			varstring_append_str(b, argv[i]);
		}
		puts(varstring_access(b));
		
		return EXIT_SUCCESS;
        	
	}
}
