#include "mymake.h"
#include "digraph.h"
#include "varstring.h"
#include "util.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct mymake_node mymake_node;

struct mymake_t
{
	FILE * output;
	FILE * error;
	digraph_t * graph;
	mymake_node ** targets;
	unsigned int numTargs;
};

struct mymake_node
{
        char * name;
        const char ** recipe;
	unsigned int recipecount;
};

void destroyFunc(void * nodedata)
{
	mymake_node * t = (mymake_node*)nodedata;
	unsigned int count = t->recipecount;
	char ** r = (char**)t->recipe;
	for (int i=0; i<count; ++i)
	{
		free(r[i]);
	}
        return;
}

mymake_t * mymake_create(FILE * output, FILE * error)
{
	mymake_t * mt = malloc(sizeof(*mt));
	mt->output = output;
	mt->error = error;

	if (mymake_supports_variables())
	{
		fprintf(mt->error, "Does not support variables");
	}

	mt->graph = digraph_create(destroyFunc);
	mt->targets = NULL;
	mt->numTargs=0;
	return mt;
}

bool mymake_supports_variables()
{
	return false;
	
}

bool mymake_add_variable(mymake_t * m, const char * varname, const char * val)
{
	return true;
}

bool findFunc(digraph_t * d, digraph_node_t * node, void * userdata)
{
        void * data = digraph_node_get_data(d, node);
	if (data)
	{
		mymake_node * digraph_data = (mymake_node*)data;
		char * digraphName = digraph_data->name;
		char * newName = (char*)userdata;
		if(strcmp(digraphName, newName)==0)
		{
			return true;
		}
	}
	return false;
}

digraph_node_t * create_mymake_node(mymake_t * m, const char * name)
{
	unsigned int count = m->numTargs;
        mymake_node * t = malloc(sizeof(*t));
        t->name = mystrdup(name);
	t->recipe = NULL;
	t->recipecount = 0;

        m->targets = realloc(m->targets, sizeof(*(m->targets))*(count+1));
        m->targets[count] = t;
        digraph_node_t * dt = digraph_node_create(m->graph, (void*)t);
        ++(m->numTargs);
	return dt;
}

bool mymake_add_target(mymake_t * m, const char * name, const char ** deps, unsigned int depcount, const char ** recipe, unsigned int recipecount)
{
	//check if node with name already exists
	digraph_node_t * tar = digraph_find(m->graph, findFunc, (void*)name);
	
	//if not create it
	if (tar==NULL)
	{
		tar = create_mymake_node(m, name);
	}

	for (int i=0; i<depcount; ++i)
	{
		digraph_node_t * dep = digraph_find(m->graph, findFunc, (void*)deps[i]);
		
		if (dep==NULL)
		{
			dep = create_mymake_node(m, deps[i]);
			digraph_add_link(m->graph, tar, dep);
		}
	}

	void * data = digraph_node_get_data(m->graph, tar);
        mymake_node * target = (mymake_node*)data;

	if(target->recipe==NULL)
	{
		target->recipecount = recipecount;
                target->recipe = realloc(target->recipe, sizeof(*(target->recipe))*(recipecount+1));
		for (int i=0; i<recipecount; ++i)
		{
			target->recipe[i] = recipe[i];
		}
	}
	else
	{
		fprintf(m->error, "Multiple recipes for same target are not allowed!");
	}
		
	return true;
}

bool mymake_build(mymake_t * m, const char * target, bool verbose, bool dryrun)
{
	digraph_node_t * build_target;
	char * targetName;
	if (target==NULL)
	{	
		char * firstTarget = m->targets[0]->name;
		build_target = digraph_find(m->graph, findFunc, (void*)firstTarget);
		if(build_target==NULL)
		{
			return true;
		}
		else
		{
			targetName = m->targets[0]->name;
		}
	}
	else
	{
		unsigned int count = m->numTargs;
		char * searchName;
		for (int i=0; i<count; ++i)
		{
			searchName = m->targets[i]->name;
			if (strcmp(searchName, target)==0)
			{
				build_target = digraph_find(m->graph, findFunc, (void*)target);
				break;
			}	
		}
		if (build_target==NULL)
		{
			fprintf(m->error, "We don't know how to build that target");
			return false;
		}
		else
		{
			targetName = searchName;
		}
	}

	unsigned int outgoing_links = digraph_node_outgoing_link_count(m->graph, build_target);
	char * depName; 

	for (int j=0; j<outgoing_links; ++j)
	{
		digraph_node_t * dep;
		bool v = digraph_node_get_link(m->graph, build_target, j, &dep);
		if (v)
		{
			void * data = digraph_node_get_data(m->graph, dep);
                	mymake_node * depNode = (mymake_node*)data;
                	depName = depNode->name;
                	mymake_build(m, depName, verbose, dryrun);
		}		
	}
	
        uint64_t lastModTar = last_modification(targetName);
	uint64_t lastModDep;
	if (depName!=NULL)
	{
		lastModDep = last_modification(depName);
	}
	
	if (lastModDep>lastModTar)
	{	
		void * d = digraph_node_get_data(m->graph, build_target);
		mymake_node * buildNode = (mymake_node*)d;
		const char ** r = buildNode->recipe;
		unsigned int rcount = buildNode->recipecount;
		for (int i=0; i<rcount; ++i)
		{
			execute_recipe(r, rcount, m->output, m->error, dryrun);
			//printf("%s\n", r[i]);
		}
	}
	return true;
}

void mymake_destroy(mymake_t * m)
{
	digraph_destroy(m->graph);
	for (int i=0; i<(m->numTargs); ++i)
	{
		free(m->targets[i]->recipe);
		free(m->targets[i]->name);
		free(m->targets[i]);
	}
	free(m->targets);
	free(m);
	return;
}
