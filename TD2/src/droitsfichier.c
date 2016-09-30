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
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	struct stat file1; 

	if(stat(argv[2], &file1) != 0) {
		printf("Impossible d'ouvrir le fichier %s\n", argv[2]);
		perror("file1");
		return errno;
	}
	if(file1.st_mode & S_IFDIR)
	{
		printf("Vous devez specifier un fichier:\n");
		return EXIT_FAILURE;
	}

	if(strcmp(argv[1], "e") == 0 || strcmp(argv[1], "E") == 0)
	{
		if(argc != 3) {
			printf("E: args\n");
	        return EXIT_FAILURE;
		}
		if(unlink(argv[2]) == -1)
		{
			perror("delete");
			return errno;
		}
	}

	else if(strcmp(argv[1], "r") == 0 || strcmp(argv[1], "R") == 0)
	{
		if(argc != 4) {
			printf("R: args\n");
	        return EXIT_FAILURE;
		}

		if(rename(argv[2], argv[3]) == -1)
		{
			perror("rename");
			return errno;
		}
	}

	else if(strcmp(argv[1], "c") == 0 || strcmp(argv[1], "C") == 0)
	{
		if(argc != 4) {
			printf("E: args\n");
	        return EXIT_FAILURE;
		}

		if(strcmp(argv[3], "r") == 0 || strcmp(argv[3], "R") == 0)
		{
			mode_t newmode = file1.st_mode|S_IRUSR;

			if(chmod(argv[2], newmode) == -1)
			{
				perror("chmodR");
				return EXIT_FAILURE;
			}
		}

		else if(strcmp(argv[3], "w") == 0 || strcmp(argv[3], "W") == 0)
		{
			mode_t newmode = file1.st_mode|S_IWUSR;
			if(chmod(argv[2], newmode) == -1)
			{
				perror("chmodW");
				return EXIT_FAILURE;
			}
		}

		else
		{
			printf("Chmod: specify 'r' or 'w' args\n");
			return EXIT_FAILURE;
		}
	}
	else
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}