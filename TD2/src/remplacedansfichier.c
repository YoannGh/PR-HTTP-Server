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
	int fd;
	int i = 0;
	int firstWordSize = 0;

	if(argc <= 4) {
		printf("not enough args\n");
	    return EXIT_FAILURE;
	}

    if ((fd = open (argv[1], O_RDWR|O_CREAT, 0600)) == -1) {
      perror("open fd");
      return errno;
    }

    firstWordSize = strlen(argv[3]);

    for(i = 3; i < argc; ++i)
    {
		if (write (fd,argv[i], strlen (argv[i])) == -1) {
			perror("fd_write");
			return errno;
		}

		printf("%s ", argv[i]);

		if (write (fd, " ", 1) == -1) {
			perror("fd_write");
			return errno;
		}
    }

    printf("\n");

  lseek(fd, firstWordSize+1, SEEK_SET);

  if((write(fd,argv[2],strlen(argv[2]))) == -1)
  {
      perror("write nouveau mot");
      return errno;
  }

	return EXIT_SUCCESS;
}