#include "makefile_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This program should use makefile_parser.h functions and output the input mymake file
 * in a standardized format.
 *
 * There should not be a blank line at the top of the file.
 *
 * Each rule (and associated recipe) should be output without extra
 * whitespace, in the order found in the input file.
 * After each rule (and optional recipe) there should be a single blank line.
 *
 * If supporting variables, each variable definition found should be output
 * in the form:
 * 
 * VARIABLE=VALUE
 *
 * (no added whitespace around =).
 * There should be no blank line after variables.
 *
 * If multitarget support was implemented, targets should be separated by
 * a single space. There should not be a space before ':'.
 *
 * If the rule has dependencies, each dependency should be prefix with a
 * single space.
 *
 * (Which means that if a rule has no dependencies, there will not be a space
 * following ':')
 *
 * You can test your output by comparing against the provided example files.
 * (You can compare using the `diff -u <file1> <file2>` command).
 *
 * Your program should take a single command line argument (and complain if more or less are provided)
 * which is the name of the input file.
 *
 * The output should be written to the screen (stdout).
 * Any parser errors should be written to stderr.
 */


int main(int argc, char ** args)
{
	if (argc != 2)
        {
                puts("Please add correct command line arguments!");
                return EXIT_FAILURE;
        }

	

        FILE *f = fopen(args[1], "r");
        if(f==NULL)
        {
                exit(EXIT_FAILURE);
        }

        mfp_cb_t * mycb = malloc(sizeof(*mycb));
        void * data;
	
	bool varFunc(void * userdata, unsigned int line, const char * varname, const char * value)
	{
		fprintf(stdout, "%s=%s\n", varname, value);
	}

	bool ruleFunc(void * userdata, const char ** target, unsigned int tcount, const char ** dependencies, unsigned int dcount, const char ** recipe, unsigned int rcount)
	{
		if (*(int*)userdata != 2)
			fprintf(stdout, "\n")**(int*)userdata;
		for(int i=0; i<tcount; ++i)
		{
			if (i==0)
			{
				fprintf(stdout, "%s", target[i]);
			}
			else
			{
				fprintf(stdout, " %s", target[i]);
			}
		}
		fprintf(stdout, ":");
		if (dependencies!=NULL)
		{
			for(int i=0; i<dcount; ++i)
			{
                                	fprintf(stdout, " %s", dependencies[i]);
			}
			fprintf(stdout, "\n");
		
			if (recipe!=NULL)
			{
				for(int i=0; i<rcount; ++i)
                		{
					fprintf(stdout, "\t%s\n", recipe[i]);
				}
			}
		}
		if (*(int*)userdata == 2)
                	fprintf(stdout, "\n")**(int*)userdata;
	}

	mycb->variable_cb = varFunc;
	mycb->rule_cb = ruleFunc;

        mfp_parse(f, mycb, data);
	free(mycb);
}
