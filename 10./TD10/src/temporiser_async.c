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
#include <time.h>
#include <semaphore.h>

sem_t fin;

struct myData
{
	char* filename;
	struct aiocb* aio;
};

void interrupt_signal(int signo, siginfo_t *si, void *context) {
	int fdread, n;
	char buffer[4096];
	struct aiocb* aio;
	struct myData* data;

	printf("signal recu\n");
	data = (struct myData*) si->si_value.sival_ptr;

	aio = data->aio;

	if(aio_error(aio) == 0) {

		printf("aiodone\n");
		
		if ((fdread = open(data->filename, O_CREAT|O_RDONLY, 0600)) < 0) {
    		perror("open file read");
    		exit(errno);
		}

		if((n=read(fdread,&buffer, 4096)) == -1)
		{
		    perror("read fd1");
		    exit(errno);
		}

		buffer[n] = '\0';

		printf("Read: %s\n", buffer);

		sem_post(&fin);

	}
}

int main(int argc, char* argv[]) {

	int fdwrite;
	struct aiocb* aiocbp;
	sigset_t block_mask;
	struct sigaction action;
	timer_t tmr_expl;
	struct sigevent signal_spec;
	struct itimerspec new_tmr, old_tmr;
	struct myData data;

	if(argc != 3) {
        printf("Bad args\n");
        return EXIT_FAILURE;
    }

    aiocbp = malloc(sizeof(struct aiocb));

    sem_init(&fin, 0, 0);

    action.sa_sigaction = interrupt_signal;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&block_mask);
    /*sigaddset(&block_mask, SIGRTMIN);*/
    action.sa_mask = block_mask;
    sigaction(SIGRTMIN, &action, 0);
    /*sigaddset(&block_mask, SIGRTMIN);
    sigprocmask(SIG_SETMASK, &block_mask, 0);*/

    data.filename = argv[1];
    data.aio = aiocbp;
    signal_spec.sigev_notify = SIGEV_SIGNAL;
	signal_spec.sigev_signo = SIGRTMIN;
	signal_spec.sigev_value.sival_ptr = (void*) &data;
	if (timer_create(CLOCK_REALTIME, &signal_spec, &tmr_expl) != 0) {
		perror("timer_create error!");
		return errno;
	}

    new_tmr.it_value.tv_sec = 0;
	new_tmr.it_value.tv_nsec = 0;
	new_tmr.it_interval.tv_sec = 0;
	new_tmr.it_interval.tv_nsec = 50;
	if (timer_settime(tmr_expl, 0, &new_tmr, &old_tmr) != 0) {
        perror("time_settime error!");
        return errno;
    }

	puts("timer started");

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

	if (aio_write(aiocbp) < 0) {
    	perror("aiowrite");
    	return errno;
	}

	puts("semwait");

	sem_wait(&fin);

	free(aiocbp);

	return 0;
}