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
	struct stat file2;

	if(argc != 3) {
		printf("usage: %s filepath1 filepath2\n", argv[0]);
        return EXIT_FAILURE;
	}

	if(stat(argv[1], &file1) != 0) {
		printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
		perror("file1");
		return errno;
	}

	if(stat(argv[2], &file2) != 0) {
		printf("Impossible d'ouvrir le fichier %s\n", argv[2]);
		perror("file2");
		return errno;
	}

	if(file1.st_ino == file2.st_ino)
		printf("Meme fichiers\n");
	else {
		printf("Fichiers differents\n");
	}

	return 0;
}