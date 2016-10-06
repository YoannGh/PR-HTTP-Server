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
#include <dirent.h>
#include <linux/limits.h>

int main(int argc, char const *argv[])
{
	int fd1, fd2;
	int filesize, resteAEcrire;
	struct stat filestat;
	char* filebuffer;

	if (argc != 2)
	{
		printf("Error args\n");
		return EXIT_FAILURE;
	} 

	if ((fd1 = open (argv[1], O_RDONLY)) == -1) {
		perror("open fd");
		return errno;
	}

	if ((fd2 = open ("invert_result_pread", O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1) {
		perror("open fd");
		return errno;
	}

	if(stat(argv[1], &filestat) != 0) {
		printf("Impossible d'ouvrir le repertoire %s\n", argv[2]);
		perror("rep");
		return errno;
	}

	filesize = filestat.st_size;

	filebuffer = malloc(filesize);

	if(pread(fd1, filebuffer, (size_t) filesize, 0) == -1) {
		perror("pread");
		return errno;
	}

	for(resteAEcrire = filesize; resteAEcrire != 0; resteAEcrire--) {
		if(pwrite(fd2, &filebuffer[resteAEcrire-1], 1, filesize - resteAEcrire) == -1) {
			perror("pwrite");
			return errno;
		}
	}

	close(fd1);
	close(fd2);
	free(filebuffer);

	return EXIT_SUCCESS;

}