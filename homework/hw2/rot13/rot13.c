#include <stdio.h>
#include <stdlib.h>
#define ROT 13

int rot13(int input, int a, int z)
{
	int rem = (input - 1 + ROT) % z;	// calc remainder off by 1 to keep m in first group
  	int quo = (input - 1 + ROT) / z;	// calc quotient off by 1 to keep m in first group
  	int rotInput = quo * a + rem + 1 - quo;	// if first group add 1, else add a
  	return rotInput;
}

int main(int argc, char * argv[])
{
	FILE *fi = fopen("rot13-in.txt", "r");	// open files
  	FILE *fo = fopen("rot13-out.txt", "w");

  char c;
  while ((c=fgetc(fi))!=EOF)	//look at each letter and apply rot13 otherwise leave it alone
  {
  	if(c >= 'a' && c <= 'z')
    	{
      		putchar(rot13(c, 'a', 'z'));
        }
    	else if(c >= 'A' && c <= 'Z')
    	{
      		putchar(rot13(c, 'A', 'Z'));
      
    	}
    	else
    	{
      		putchar(c);
       	}
  }
  fclose(fi);	//close files
  fclose(fo);

  return EXIT_SUCCESS;
}
