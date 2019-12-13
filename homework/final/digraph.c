#include "digraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct digraph_node_t
{
	void * data;
	unsigned int incoming_links;
	unsigned int outgoing_links;
	digraph_node_t ** children;
};

struct digraph_t
{
	void * tempData;
	unsigned int outgoing_links;
	digraph_node_t ** children;
	digraph_destroy_cb_t des;
};

digraph_node_t ** get_children(digraph_t * d, digraph_node_t * nt)
{
	if (nt!=NULL)
		return nt->children;
	return d->children;
}

void increment_outgoing_links(digraph_t * dt, digraph_node_t * nt)
{
	if (nt!=NULL)
		++(nt->outgoing_links);
	else
		++(dt->outgoing_links);
}

void digraph_add_link(digraph_t * d, digraph_node_t * from, digraph_node_t * to)
{
	unsigned int c = digraph_node_outgoing_link_count(d, from);
	if (from!=NULL)
	{
		from->children = realloc(from->children, sizeof(*(from->children))*(c+1));
        	from->children[c] = to;
	}
	else
	{
		d->children = realloc(d->children, sizeof(*(d->children))*(c+1));
		d->children[c] = to;
	}

	increment_outgoing_links(d, from);
        ++(to->incoming_links);
}

void digraph_node_destroy(digraph_t * d, digraph_node_t * n)
{
	digraph_destroy_cb_t desFunc = d->des;
	if(desFunc!=NULL && n->data!=NULL)
	{
		desFunc(n->data);
	}
	free(n->children);
	free(n);
}

void digraph_destroy(digraph_t * graph)
{
	digraph_node_t ** children = get_children(graph, NULL);
        unsigned int c = digraph_node_outgoing_link_count(graph, NULL);

	for (unsigned int i=0; i<c; ++i)
        {
		digraph_node_destroy(graph, children[i]);
	}
	free(graph->children);
	free(graph);	
}

bool digraph_node_visit(digraph_t * d, digraph_node_t * n, digraph_visit_cb_t visit, void * userdata)
{
	digraph_node_t ** children = get_children(d, n);
	unsigned int c = digraph_node_outgoing_link_count(d, n);
	
	if(children!=NULL)
	{
		for (unsigned int i=0; i<c; ++i)
		{
			if(visit!=NULL)
			{
				bool v = visit(d, children[i], userdata);
				if (!v)
					return false;
			}
		}
	}

	return true;
}

bool digraph_visit(digraph_t * g, digraph_visit_cb_t cb, void * userdata)
{
	return digraph_node_visit(g, NULL, cb, userdata);
}

digraph_node_t * digraph_find(digraph_t * g, digraph_visit_cb_t cb, void * userdata)
{
	digraph_node_t ** children = get_children(g, NULL);
        unsigned int c = digraph_node_outgoing_link_count(g, NULL);

        if(children!=NULL)
        {
                for (unsigned int i=0; i<c; ++i)
                {
                        if(cb!=NULL)
                        {
                                bool v = cb(g, children[i], userdata);
                                if (v)
                                        return children[i];
                        }
                }
        }
        return NULL;
}

bool digraph_node_get_link(digraph_t * d, digraph_node_t * n, unsigned int idx, digraph_node_t ** ret)
{
	digraph_node_t ** children = n->children;
	if(children[idx])
	{
		*ret = children[idx];
		return true;
	}
	return false;
}

digraph_t * digraph_create(digraph_destroy_cb_t cb)
{
	digraph_t * dt = malloc(sizeof(*dt));
	dt->tempData = NULL;
	dt->outgoing_links=0;
	dt->children = NULL;
	dt->des = cb;
	return dt;
}

unsigned int digraph_node_outgoing_link_count(const digraph_t * d, const digraph_node_t * n)
{
	if (n!=NULL)
                return n->outgoing_links;
        return d->outgoing_links;
}

unsigned int digraph_node_incoming_link_count(const digraph_t * d, const digraph_node_t * n)
{
	return n->incoming_links;
}

digraph_node_t * digraph_node_create(digraph_t * d, void * userdata)
{
	digraph_node_t * nt = malloc(sizeof(*nt));
	nt->data = userdata;
	nt->incoming_links=0;
	nt->outgoing_links=0;
	nt->children = NULL;

	//add to graph
	digraph_add_link(d, NULL, nt);
	return nt;
}

//FIXME could potentially save it in d
void * digraph_node_set_data(digraph_t * d, digraph_node_t * n, void * userdata)
{
	//void * temp = malloc(sizeof(int));
	//memcpy(temp, n->data, sizeof(int));
	//d->tempData = n->data;
	n->data = userdata;
	return n->data;
}

void * digraph_node_get_data(const digraph_t * d, const digraph_node_t * n)
{
	return n->data;
}

