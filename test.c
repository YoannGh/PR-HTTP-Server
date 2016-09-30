#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>


int main(int argc, char const **argv)
{

	printf("stdout fd: %d\n", STDOUT_FILENO);
	return 0;
}
