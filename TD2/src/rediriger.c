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

int Rediriger_stdout(int fd)
{
	return dup2(fd,STDOUT_FILENO);
}

int Restaurer_stdout(int fd)
{
	return dup2(fd, STDOUT_FILENO);
}


int main(int argc, char const *argv[])
{
	int fd, fdsave;
	fdsave = dup(STDOUT_FILENO);

	if(argc != 2) {
		printf("args\n");
	    return EXIT_FAILURE;
	}

	if ((fd = open (argv[1], O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1) {
      perror("open fd");
      return errno;
    }

    printf("Avant_redirection\n");
    Rediriger_stdout(fd);
    printf("Apres_redirection\n");
    printf("Avant_restauration\n");
    Restaurer_stdout(fdsave);
    printf("Apres_restauration\n");

    close(fd);

	return EXIT_SUCCESS;
}