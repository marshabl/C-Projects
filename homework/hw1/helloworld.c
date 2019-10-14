#include <stdio.h>
#include <stdlib.h>

int ret ()
{
	return printf("Hello, world!\n");
}

int main (int argc, char ** args)
{
	ret();
	return EXIT_SUCCESS;
}
