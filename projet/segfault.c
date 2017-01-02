#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	char* c;
	c = NULL;
	*c = "segfault";
	return EXIT_SUCCESS;
}