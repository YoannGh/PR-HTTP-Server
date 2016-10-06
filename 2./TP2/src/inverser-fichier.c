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
	int ecris = 0, filesize;
	char c;

	if (argc != 2)
	{
		printf("Error args\n");
		return EXIT_FAILURE;
	} 

	if ((fd1 = open (argv[1], O_RDONLY)) == -1) {
      perror("open fd");
      return errno;
    }

	if ((fd2 = open ("invert_result", O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1) {
      perror("open fd");
      return errno;
    }

	filesize = lseek(fd1, -1, SEEK_END) + 1;
	while((read(fd1, &c, 1) != 0) && (ecris != filesize)) {
		if ((ecris += write (fd2, &c, 1)) == -1) {
			perror("fd_write");
			return errno;
		}

		lseek(fd1, -2, SEEK_CUR);
	}

	close(fd1);
	close(fd2);

	return EXIT_SUCCESS;
}
