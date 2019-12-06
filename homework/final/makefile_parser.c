#include "makefile_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prepFileForParse(FILE * inputFile, FILE * outputFile)
{       
        int firstChar = 0;
        char c;
        
        while ((c=fgetc(inputFile))!=EOF)
        {       
                if(c=='\n' && firstChar==0)
                {       
                        firstChar=1;
                        fputc(c, outputFile);
                        continue;
                }
                else if(firstChar==1)
                {       
                        if (c=='\n' || c==' ')
                        {       
                                while((c=fgetc(inputFile))!=EOF && c=='\n')
                                {       
                                        continue;
                                }
                        }
                        else
                        {
                                firstChar=0;
                                fputc(c, outputFile);
                                continue;
                        }
                }
                if (c=='#')
                {
                        while((c=fgetc(inputFile))!=EOF && c!='\n')
                        {
                                continue;
                        }
                        if (firstChar==1)
                        {
                                firstChar=0;
                                continue;
                        }
                }
                fputc(c, outputFile);
        }
}

char * mystrdup (const char * s)
{
   char * n = (char *) malloc (strlen(s)+1);
   strcpy (n, s);
   return n;
}

char * newArray()
{
        char * newArray = malloc(sizeof(1));
        *newArray = '\0';
        return newArray;
}

char * append_char(char * str, char c)
{
        unsigned int len = strlen(str);
        str = realloc(str, sizeof(*str)*(len+2));
        str[len] = c;
        str[len+1] = '\0';
        return str;
}

char ** add_word(char ** array, unsigned int * wordcount, const char * word_to_add)
{
        char * copyword = mystrdup(word_to_add);
        char **newarray = realloc(array, sizeof(*newarray)*(*wordcount+1));
        newarray[*wordcount] = copyword;
        return newarray;
}

const char ** copy_array(char ** oldarray, unsigned int wordcount)
{
	const char ** newarray = malloc(sizeof(*oldarray)*(wordcount));
        for (int h=0; h<wordcount; ++h)
        {
        	newarray[h] = oldarray[h];
        }
	return newarray;
}

void free_word_array(char ** array, unsigned int wordcount)
{
        for (unsigned int i=0; i<wordcount; ++i)
        {
                free(array[i]);
        }
        free(array);
}

char * load_stage(char * stage, char *c, FILE * f)
{
        while((*c=fgetc(f))!=EOF && *c!='\n')
                stage = append_char(stage, *c);
        return stage;
}

char * resetStage(char * stage)
{
        free(stage);
        stage = newArray();
        return stage;
}

char ** getMult(char ** array, char * str, int * i, int * count)
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
        
        while((*c=fgetc(f))!=EOF && *c!='\n')
        {       
                str = append_char(str, *c);
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

char ** getTarOrDep(char * str, int * wordcount)
{
	char ** tarOrDep = malloc(sizeof(*tarOrDep));
        int i = 0;
        tarOrDep = getMult(tarOrDep, str, &i, wordcount);
	return tarOrDep;
}



bool mfp_parse(FILE * f, const mfp_cb_t * cb, void * extradata)
{
        FILE *nf = fopen("stageFile.txt", "w");
        if(nf==NULL)
        {
                exit(EXIT_FAILURE);
        }

        prepFileForParse(f, nf);
        fclose(f);
        fclose(nf);

        FILE *b = fopen("stageFile.txt", "r");
        if(nf==NULL)
        {
                exit(EXIT_FAILURE);
        }

        char c;
        char *stage = newArray();
        char ** identifiers = malloc(sizeof(*identifiers));
        char ** values = malloc(sizeof(*values));
        int idWordCount = 0;
        int valuesWordCount = 0;
        int lineNumber = 0;
	int firstLine = 0;

        while ((c=fgetc(b))!=EOF)
        {
                if (c=='=')
                {
			if (firstLine==0)
				firstLine=1;
                        //grab next id and check if its legal
                        identifiers = add_word(identifiers, &idWordCount, stage);
                        if(checkIds(identifiers[idWordCount])==0)
                        {
                                return false;
                        }

                        //reset stage string, then load it and add it to values
                        stage = resetStage(stage);
                        stage = load_stage(stage, &c, b);
                        values = add_word(values, &valuesWordCount, stage);

                        //call mfp_variable_cb_t
                        mfp_variable_cb_t v = cb->variable_cb;
			v(extradata, lineNumber, identifiers[idWordCount], values[valuesWordCount]);
                        if(v==0)
                        {
                                return false;
                        }
			++lineNumber;
			++idWordCount;
			++valuesWordCount;

                        //reset stage and continue loop
                        stage = resetStage(stage);
                        continue;
                }
		else if (c==':')
                {
			if (firstLine==0)
                                firstLine=2;
			int targetWordCount = 0;
			char ** target = getTarOrDep(stage, &targetWordCount);
			
			for (int b=0; b<targetWordCount; ++b)
        		{       
                		bool t = checkTarDep(target[b]);
                		if (!t)  
                        		return false;
        		}

                        //reset stage for dependencies
                        stage = resetStage(stage);
                        stage = load_stage(stage, &c, b);

			int depWordCount = 0;
			char ** dependencies = getTarOrDep(stage, &depWordCount);
			for (int b=0; b<depWordCount; ++b)
                        {
                                bool t = checkTarDep(dependencies[b]);
                                if (!t)
                                        return false;
                        }

			char ** recipe = malloc(sizeof(*recipe));
			int recWordCount = 0;
			++lineNumber;
			if ((c=fgetc(b))!=EOF && c=='\t')
			{
				recipe = getRecipe(b, &c, &recWordCount, recipe);
				for (int i=0; i<recWordCount; ++i)
				{
					++lineNumber;
				}
			}
			else
			{
				ungetc(c, b);
			}
	
			const char ** newt = copy_array(target, targetWordCount);
			const char ** newd = copy_array(dependencies, depWordCount);
			const char ** newr = copy_array(recipe, recWordCount);
			
			mfp_rule_cb_t r = cb->rule_cb;
			extradata = (void *)&firstLine;
                        r(extradata, newt, targetWordCount, newd, depWordCount, newr, recWordCount);
                        if(r==0)
                        {
                                return false;
                        }
			
                        //reset stage and continue the loop
                        stage = resetStage(stage);
			
			free_word_array(dependencies, depWordCount);
			free_word_array(target, targetWordCount);
			free_word_array(recipe, recWordCount);
			free((char**)newt);
			free((char**)newr);
			free((char**)newd);
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
        fclose(b);
        return true;
}

