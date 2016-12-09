#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <aio.h>

int main(int argc, char* argv[]) {

	int fdwrite, fdread, sigval;
	struct aiocb* aiocbp;
	static struct aiocb*  aio_list[1];
	char buffer[4096];

	if(argc != 3) {
        printf("Bad args\n");
        return EXIT_FAILURE;
    }

    aiocbp = malloc(sizeof(struct aiocb));


	if ((fdwrite = open (argv[1], O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1) {
    	perror("open fd");
    	return errno;
    }

	aiocbp->aio_fildes = fdwrite;
	aiocbp->aio_buf = argv[2];
	aiocbp->aio_nbytes = strlen(argv[2]);
	aiocbp->aio_offset = 0;
	aiocbp->aio_reqprio = 0;

	aio_list[0] = aiocbp;

	if (aio_write(aiocbp) < 0) {
    	perror("aiowrite");
    	return errno;
	}

	puts("avant read");

	if ((fdread = open(argv[1], O_CREAT|O_RDONLY, 0600)) < 0) {
    	perror("open file read");
    	return errno;
	}

	if (aio_suspend(aio_list, 1, 0) == -1) {
		perror("aio_suspend");
		exit(1);
	}


	aiocbp->aio_fildes = fdread;
	aiocbp->aio_buf = buffer;
	aiocbp->aio_nbytes = 4096;

	if (aio_read(aiocbp) < 0) {
    	perror("aiowrite");
    	return errno;
	}

	if (aio_suspend(aio_list, 1, 0) == -1) {
		perror("aio_suspend");
		exit(1);
	}
	printf("buffer: %s\n", buffer);

	free(aiocbp);

	return 0;
}