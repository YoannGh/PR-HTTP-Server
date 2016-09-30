#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	struct stat* file1, file2;

	if(argc != 3) {
		printf("usage: %s filepath1 filepath2\n", argv[0]);
        return EXIT_FAILURE;
	}

	if(stat(argv[1], &file1) != 0) {
		
	}
	return 0;
}