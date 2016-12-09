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

void interrupt_signal(int signo, siginfo_t *si, void *context){
	printf("signal recu\n");
}

int main(int argc, char* argv[]) {

	int fdwrite, fdread, n, sigval;
	struct aiocb* aiocbp;
	sigset_t block_mask;
	struct sigaction action;
	char buffer[4096];

	if(argc != 3) {
        printf("Bad args\n");
        return EXIT_FAILURE;
    }

    aiocbp = malloc(sizeof(struct aiocb));

    action.sa_sigaction = interrupt_signal;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&block_mask);
    action.sa_mask = block_mask;
    sigaction(SIGRTMIN, &action, 0);
    sigaddset(&block_mask, SIGRTMIN);
    sigprocmask(SIG_SETMASK, &block_mask, 0);

	if ((fdwrite = open (argv[1], O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1) {
    	perror("open fd");
    	return errno;
    }

	aiocbp->aio_fildes = fdwrite;
	aiocbp->aio_buf = argv[2];
	aiocbp->aio_nbytes = strlen(argv[2]);
	aiocbp->aio_offset = 0;
	aiocbp->aio_reqprio = 0;
	aiocbp->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	aiocbp->aio_sigevent.sigev_signo = SIGRTMIN;
	aiocbp->aio_lio_opcode = LIO_WRITE;

	printf("writen: %d\n", n);

	if (aio_write(aiocbp) < 0) {
    	perror("aiowrite");
    	return errno;
	}

	puts("avant read");

	if ((fdread = open(argv[1], O_CREAT|O_RDONLY, 0600)) < 0) {
    	perror("open file read");
    	return errno;
	}

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGRTMIN);

	if (sigwait(&block_mask, &sigval) == -1) {
		perror("sigwait");
		exit(1);
	}

	if((n=read(fdread,&buffer, 4096)) == -1)
	{
	    perror("read fd1");
	    return errno;
	}

	buffer[n] = '\0';

	printf("Read: %s\n", buffer);

	free(aiocbp);

	return 0;
}