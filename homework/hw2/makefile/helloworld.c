#include <stdio.h>
#include <stdlib.h>

int ret ()
{
	return printf("Hello, world! It is a wonderful day!\n");
}

int main (int argc, char ** args)
{
	ret();
	return EXIT_SUCCESS;
}
