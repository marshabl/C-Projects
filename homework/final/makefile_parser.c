#include "makefile_parser.h"
#include "varstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * load_stage(char * stage, char *c, FILE * f)
{
	unsigned int inComment = 0;
        while((*c=fgetc(f))!=EOF && *c!='\n')
	{
		if (*c=='#')
                {
			inComment=1;
			continue;
                }
		else if (inComment == 1)
		{
			continue;
		}
                stage = append_char(stage, *c);
		inComment =0;
	}
        return stage;
}

char * resetStage(char * stage)
{
        free(stage);
        stage = newArray();
        return stage;
}

char ** getMult(char ** array, char * str, unsigned int * i, unsigned int * count)
{       
        unsigned int len = strlen(str);
        char * nstr = newArray();
        
        for (; *i<len; ++(*i))
        {       
                if (str[*i]=='\0')
                        return array;
                if (str[*i]==' ')
                        break;
                nstr = append_char(nstr, str[*i]);
        }
        if(nstr[0]!='\0')
        {       
                array = add_word(array, count, nstr);
                ++(*count);
        }
        free(nstr);
        if (str[*i]==' ')
        {       
                ++(*i); 
                array = getMult(array, str, i, count);
        }
        return array;
}

void skipBlankLines(FILE*f, char * c)
{       
        while(*c!=EOF && *c!='\n')
        {       
                ;
        }
        
        *c=fgetc(f); 
        if (*c=='\n' || *c==' ')
        {       
                skipBlankLines(f, c);
        }
}

char ** getRecipe(FILE*f, char * c, unsigned int * wordcount, char ** recipe)
{       
        char * str = newArray();
	unsigned int inComment = 0;
        
        while((*c=fgetc(f))!=EOF && *c!='\n')
        {
		if (*c=='#')
                {
                        inComment=1;
                        continue;
                }
                else if (inComment == 1)
                {
                        continue;
                }       
                str = append_char(str, *c);
		inComment =0;
        }
        
        if (wordcount==0)
        {       
                recipe[*wordcount]=str;
        }
        else
        {       
                recipe = add_word(recipe, wordcount, str);
        }
        free(str);
        ++(*wordcount);
        
        *c=fgetc(f);
        
        if (*c=='\n' || *c==' ')
        {     
              skipBlankLines(f, c);
        }
        
        if (*c!=EOF && *c=='\t')
        {       
                recipe = getRecipe(f, c, wordcount, recipe);
        }
        else
        {       
                ungetc(*c, f);
        }
        return recipe;
}

bool checkIds(char * id)
{
        char checkFirst[54] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_\0";
        char checkRest[64] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_\0";
        unsigned int len = strlen(id);
        for(int i=0; i<len; ++i)
        {
                if(i==0)
                {
                        for (int j=0; j<53; ++j)
                        {
                                if (id[i]==checkFirst[j])
                                {
                                        break;
                                }
                                else if (j==52 && id[i]!=checkFirst[j])
                                {
                                        return false;
                                }
                        }
                }
                for (int k=0; k<63; ++k)
                {
                        if (id[i]==checkRest[k])
                        {
                                break;
                        }
                        else if (k==62 && id[i]!=checkRest[k])
                        {
                                return false;
                        }
                }
        }
        return true;
}

bool checkTarDep(char * tarDep)
{
        char checkAll[68] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 _.-/\0";
        unsigned int len = strlen(tarDep);
        for(int i=0; i<len; ++i)
        {
                for (int k=0; k<67; ++k)
                {
                        if (tarDep[i]==checkAll[k])
                        {
                                break;
                        }
                        else if (k==66 && tarDep[i]!=checkAll[k])
                        {
                                return false;
                        }
                }
        }
        return true;

}

char ** getTarOrDep(char * str, unsigned int * wordcount)
{
	char ** tarOrDep = malloc(sizeof(*tarOrDep));
        unsigned int i = 0;
        tarOrDep = getMult(tarOrDep, str, &i, wordcount);
	return tarOrDep;
}

void skipComments(FILE * f, char * c)
{
	while((*c=fgetc(f))!='\n')
        {       
                continue;
        }
        *c=fgetc(f);
        if(*c=='#')
        {       
                skipComments(f, c);
        }
	else
	{
		ungetc(*c, f);
	}
}

bool mfp_parse(FILE * f, const mfp_cb_t * cb, void * extradata)
{
        char c;
        char *stage = newArray();
        char ** identifiers = malloc(sizeof(*identifiers));
        char ** values = malloc(sizeof(*values));
        unsigned int idWordCount = 0;
        unsigned int valuesWordCount = 0;
        int lineNumber = 0;

        while ((c=fgetc(f))!=EOF)
        {
		if (c=='#')
                {
			skipComments(f, &c);
			continue;
		}
		else if (c=='\n')
		{
			continue;
		}
                else if (c=='=')
                {
                        //grab next id and check if its legal
                        identifiers = add_word(identifiers, &idWordCount, stage);
                        if(checkIds(identifiers[idWordCount])==0)
                        {
				fprintf(cb->error, "Invalid identifier.");
                                return false;
                        }

                        //reset stage string, then load it and add it to values
                        stage = resetStage(stage);
                        stage = load_stage(stage, &c, f);
                        values = add_word(values, &valuesWordCount, stage);

			//FIXME
                        //call mfp_variable_cb_t
                        //mfp_variable_cb_t v = cb->variable_cb;
			//v(extradata, lineNumber, identifiers[idWordCount], values[valuesWordCount]);
                        //if(v==0)
                        //{
			//	fprintf(cb->error, "Variable call back failed.");
                        //        return false;
                       // }
			++lineNumber;
			++idWordCount;
			++valuesWordCount;

                        //reset stage and continue loop
                        stage = resetStage(stage);
                        continue;
                }
		else if (c==':')
                {
			unsigned int targetWordCount = 0;
			char ** target = getTarOrDep(stage, &targetWordCount);
			
			for (int b=0; b<targetWordCount; ++b)
        		{       
                		bool t = checkTarDep(target[b]);
                		if (!t)  
				{
					fprintf(cb->error, "Invalid target");
                        		return false;
				}
        		}

                        //reset stage for dependencies
                        stage = resetStage(stage);
                        stage = load_stage(stage, &c, f);

			unsigned int depWordCount = 0;
			char ** dependencies = getTarOrDep(stage, &depWordCount);
			for (int b=0; b<depWordCount; ++b)
                        {
                                bool t = checkTarDep(dependencies[b]);
                                if (!t)
				{
					fprintf(cb->error, "Invalid dependency");
                                        return false;
				}
                        }

			char ** recipe = malloc(sizeof(*recipe));
			unsigned int recWordCount = 0;
			++lineNumber;
			if ((c=fgetc(f))!=EOF && c=='\t')
			{
				recipe = getRecipe(f, &c, &recWordCount, recipe);
				for (int i=0; i<recWordCount; ++i)
				{
					++lineNumber;
				}
			}
			else
			{
				ungetc(c, f);
			}
	
			mfp_rule_cb_t r = cb->rule_cb;
                        r(extradata, (const char**)target, targetWordCount, (const char**)dependencies, depWordCount, (const char**)recipe, recWordCount);
                        if(r==0)
                        {
				fprintf(cb->error, "Rule call back failed.");
                                return false;
                        }
			
                        //reset stage and continue the loop
                        stage = resetStage(stage);
			
                        continue;
                }
                else
                {
                        stage = append_char(stage, c);
                }
        }

        //free everything and close the file
        free_word_array(identifiers, idWordCount);
        free_word_array(values, valuesWordCount);
        free(stage);
        return true;
}

