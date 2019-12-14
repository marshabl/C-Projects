#define _XOPEN_SOURCE
#include "mymake.h"
#include "makefile_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool ruleFunc(void * userdata, const char ** target, unsigned int tcount, const char ** dependencies, unsigned int dcount, const char ** recipe, unsigned int rcount)
{
	mymake_add_target((mymake_t*)userdata, target[0], dependencies, dcount, recipe, rcount);
	return true;
}

int main(int argc, char ** argv)
{
	extern char *optarg;
	extern int optind;
	int c;	
	char * fname;
	int vflag=0;
	int nflag=0;
	int fflag=0;
	FILE *f;

	while ((c = getopt (argc, argv, "vnhf:")) != -1)
	{
		switch (c)
		{
			case 'h':
				printf("Usage: mymake [-f filename] [-v] [-n] target...\n");
				break;
			case 'v':
				vflag=1;
				break;
			case 'n':
				nflag=1;
				break;
			case 'f':
				fflag=1;
				fname = optarg;
				break;
		}
	}

	if (fflag==1)
	{
		f = fopen(fname, "r");

		if(f==NULL)
        	{
			fprintf(stdout, "File doesn't exist\n");
                	exit(EXIT_FAILURE);
        	}
	}

	mymake_t * m = mymake_create(stdout, stderr);
	mfp_cb_t * mycb = malloc(sizeof(*mycb));
	mycb->rule_cb = ruleFunc;
	mycb->error = stderr;
	mfp_parse(f, mycb, (void*)m);

	if (optind < argc)
        {
                for (; optind < argc; optind++)
                {
			mymake_build(m, argv[optind], vflag, nflag);
                }
        }

	mymake_destroy(m);

	free(mycb);
	fclose(f);
	return EXIT_SUCCESS;
}
