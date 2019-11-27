#include "trie.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

struct trie_data_t
{
	unsigned int size;
	trie_pos_t root;
};

struct trie_node_t
{
	char key;
        void * value;
	trie_pos_t parent;
	trie_pos_t child;
	trie_pos_t sibling;
	trie_pos_t lastsib;
};

bool trie_recurse_destroy(trie_pos_t start, trie_free_t freefunc)
{
	trie_pos_t c;
	trie_pos_t next;
	
        for (c=start; c!=NULL; c=next)
        {
        	if (c->child)
                {
              		trie_recurse_destroy(c->child, freefunc);
                }
		next = c->sibling;
		if(freefunc && c->key=='\0')
			freefunc(c->value);
                free(c);
        }
	return true;
}

bool trie_recurse_walk(trie_pos_t start, trie_t trie, trie_walk_t walkfunc, void * priv)
{
	if (trie_size(trie) == 0)
	{
		return true;
	}

        trie_pos_t c;
        trie_pos_t next;

        for (c=start; c!=NULL; c=next)
        {
                if (c->child)
                {
                        trie_recurse_walk(c->child, trie, walkfunc, priv);
                }
                next = c->sibling;
		
               	bool v = walkfunc(trie, c, &(c->key), priv);
		if(!v)
			return false;
        }
        return true;
}


bool trie_walk(trie_t trie, trie_walk_t walkfunc, void * priv)
{
	return true;// trie_recurse_walk(trie->root, trie, walkfunc, priv);
}

void trie_destroy (trie_t trie, trie_free_t freefunc)
{
	trie_pos_t l = trie->root;
        trie_recurse_destroy(l, freefunc);
	free(trie);
	return;
}

void * trie_get_value (const trie_t trie, trie_pos_t pos)
{
	return pos->value;
}

void trie_set_value (trie_t trie, trie_pos_t pos, void * value)
{
	pos->value = value; 
}

trie_t trie_new ()
{
	trie_t t = malloc(sizeof(*t));
	assert(t);

	t->size = 0;
	t->root = NULL;
	
	return t;
}

unsigned int trie_size (const trie_t trie)
{
	return trie->size;
}

bool trie_insert (trie_t trie, const char * str, void * newval, trie_pos_t * newpos)
{
	
	if (trie_find(trie, str))
	{
		if (newpos != NULL)
		{
			*newpos = trie_find(trie, str);
		}
		return false;
	}
	
	unsigned int len = strlen(str);
	++(trie->size);
	
	if (trie->root == NULL)
	{
		trie_pos_t last = NULL;
	
		for (unsigned int i=0; i<len+1; ++i)
        	{
			trie_pos_t n = malloc(sizeof(*n));
			
			n->child=NULL;
			n->sibling=NULL;
			n->lastsib=NULL;

			//first goes to root
			if (i==0)
			{
				n->parent=NULL;
				n->key=str[i];
				n->value=NULL;
				trie->root=n;
			}
			//last gets null terminator and value
			else if (i==len)
			{
				n->parent=last;
				n->key = '\0';
				n->value = newval;
				last->child=n;
			}
			//otherwise 
			else
			{
				n->parent=last;
				n->key = str[i];
                                n->value = NULL;
				last->child=n;
			}
			
			last = n;
        	}
		goto np;
	
	}
	else
	{
		trie_pos_t l = trie->root;		
		unsigned int i=0;
		
		for (;;)
		{
			trie_pos_t g = NULL;
			trie_pos_t c;
			trie_pos_t s;

			for (c = l; c != NULL; c=c->sibling)
                	{
                        	if (c->key == str[i])
                        	{
					g = c;
					break;
				}
				if (c!=NULL)
				{
					s = c;
				}
			}
			
			if (g == NULL)
			{
				unsigned int len = strlen(str)-i;
				trie_pos_t last = NULL;
				
				for (unsigned int j=0; j<len+1; ++j)
				{
					trie_pos_t n = malloc(sizeof(*n));
					n->child=NULL;
                        		n->sibling=NULL;

					if (j==0)
					{
						n->parent=NULL;
						n->key=str[j+i];
                                		n->value=NULL;
                                		s->sibling=n;
						n->lastsib=s;
					}

					else if (j==len)
                        		{
						n->parent=last;
						n->key = '\0';
                                		n->value = newval;
                                		last->child=n;	
                        		}
                        		else
					{
						n->parent=last;
						n->key = str[j+i];
                                		n->value = NULL;
                                		last->child=n;
					}
					last = n;
				}
				goto np;				
			}
			l = g->child;
			++i;
		}
	}
	np:
		if (newpos != NULL)
		{
			*newpos = trie_find(trie, str);
			return true;
		}
		return true;
}

trie_pos_t trie_find (const trie_t trie, const char * key)
{
	trie_pos_t l = trie->root;
	
	unsigned int i=0;

	for (;;)
	{
		trie_pos_t g = NULL;
		trie_pos_t c;
		
		for (c = l; c != NULL; c=c->sibling)
		{
			if (c->key == key[i])
			{
				g = c;
				break;
			}
		}
		if (g == NULL)
		{
			return TRIE_INVALID_POS;			
		}
		
		if (key[i]=='\0')
		{
			return l;
		}
		l=g->child;
		++i;
	}
	
	return TRIE_INVALID_POS;
}

bool trie_remove (trie_t trie, const char * key, void ** data)
{
	trie_pos_t b = trie_find(trie, key);
	
	if (b==TRIE_INVALID_POS)
	{
		return false;
	}
	
	--(trie->size);

	if (data!=NULL)
	{
		trie_pos_t t = trie_find(trie, key);
		*data = t->value;
	}
	
	trie_pos_t next;

	for (; b!=NULL; )
	{
		if (b->parent)
			next = b->parent;

		if(b->sibling==NULL)
		{
			b->value=NULL;
			next->child=NULL;
			if(b->parent==NULL)
				free(b);
				return true;
			free(b);
			b=next;
		}
		else
		{
			
			if (b->parent)
			{
				b->sibling->parent = b->parent;
				b->parent->child=b->sibling;
				free(b);
			}
			
			else
			{
				b->lastsib->sibling = b->sibling;
				free(b);
			}
			
			return true;
		}
	}
	return true;
}
