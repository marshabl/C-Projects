#include "varstring.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// We need to define the varstring_t structure which was declared in the
// header before we can use it...
struct varstring_t
{
	char * string;
	char * regstr;
	unsigned int len;
};

varstring_handle varstring_new()
{
	varstring_handle newstring = malloc(sizeof(varstring_t));
  	newstring->len = 0;
  	newstring->string = malloc(1);
	newstring->regstr = malloc(1);
	newstring->regstr[0] = '\0';
  	return newstring;
}

varstring_handle varstring_from(const char * str)
{
	varstring_handle newstring = malloc(sizeof(varstring_t));
	unsigned int len = strlen(str);
	newstring->len = len;
  	newstring->string = malloc(len);
	newstring->regstr = malloc(len+1);

  	strncpy(newstring->string, str, len);
	strncpy(newstring->regstr, str, len);
	newstring->regstr[len] = '\0';
  	return newstring;
}

unsigned int varstring_length (varstring_const_handle h)
{
	return h->len;
}

void varstring_append_str(varstring_handle h, const char * c)
{
	if (c == NULL)
	{
		;
	}
	else
	{
		unsigned int len = strlen(c);
        	unsigned int varlen = varstring_length(h);
        	h->string = realloc(h->string, len+varlen);
		h->regstr = realloc(h->regstr, (len+varlen+1));
        	for (int i=0; i<len; ++i)
        	{
                	h->string[varlen+i] = c[i];
			h->regstr[varlen+i] = c[i];
        	}
		h->regstr[varlen+len] = '\0';
		h->len = varlen + len;
	}
}

void varstring_free (varstring_handle h)
{
	free(h->string);
  	free(h);
}

const char * varstring_access (varstring_const_handle handle)
{
	return handle->regstr;
}

int varstring_get_char (varstring_const_handle h, unsigned int position)
{
	if (position < 0 || position >= h->len)
	{
		return -1;
	}
	else
	{
		return h->string[position];
	}
}

bool varstring_set_char (varstring_handle h, unsigned int position, char c)
{
	if (position < 0 || position >= h->len)
        {
                return -1;
        }
        else
        {
		h->string[position] = c;
                return 1;
        }	
}

char * varstring_convert (varstring_const_handle handle)
{
  	return handle->regstr;
}


void varstring_append_char (varstring_handle h, char c)
{
	unsigned int len = h->len;
  	h->string = realloc(h->string, len+1);
  	h->regstr = realloc(h->regstr, len+2);
  	h->string[len] = c;
  	h->regstr[len] = c;
  	h->regstr[len+1] = '\0';
  	h->len = len + 1;
}


void varstring_append_varstring (varstring_handle h, varstring_const_handle other)
{
	unsigned int len = h->len;
  	unsigned int varlen = varstring_length(other);
  	h->string = realloc(h->string, (len+varlen));
  	h->regstr = realloc(h->regstr, (len+varlen+1));
  	for (int i=0; i<varlen; ++i)
  	{
    		h->string[len+i] = other->string[i];
    		h->regstr[len+i] = other->regstr[i];
  	}
  	h->regstr[varlen+len] = '\0';
  	h->len = varlen + len;
}

void varstring_set_string (varstring_handle h, const char * c)
{
	unsigned int len = strlen(c);
    	h->string = realloc(h->string, (len));
    	h->regstr = realloc(h->regstr, (len+1));
    	for (int i=0; i<len; ++i)
    	{
      		h->string[i] = c[i];
      		h->regstr[i] = c[i];
    	}
    	h->regstr[len] = '\0';
    	h->len = len;
}

bool varstring_insert_str(varstring_handle h, unsigned int position, const char * str)
{
	if (position < 0 || position >= h->len)
  	{
    		return 0;
  	}
  	else
  	{
    		unsigned int varlen = h->len;
    		unsigned int len = strlen(str);
    		char * k = malloc(varlen - position+1);
    		for (int i=0; i<varlen-position; ++i)
    		{
      			k[i] = h->string[position+i];
    		}
		k[varlen-position]='\0';

    		h->string = realloc(h->string, (position + len));
    		h->regstr = realloc(h->regstr, (position + len+1));

    		for (int i=0; i<len; ++i)
    		{
      			h->string[position+i] = str[i];
      			h->regstr[position+i] = str[i];
    		}

    		h->regstr[position + len] = '\0';
    		h->len = position + len;

    		varstring_append_str(h, k);

    		return 1;
  	}
}
