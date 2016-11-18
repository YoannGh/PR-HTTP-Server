#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

sem_t* barrier;
sem_t* truc;

void wait_barrier(int NB_PCS) {
	int i;
	if(!sem_trywait(truc))
		sem_wait(barrier);
	else
	{
		for(i=0;i<NB_PCS-1;++i)
		{
			sem_post(barrier);
			sem_post(truc);
		}
	}

}

void process (int NB_PCS) {
   puts("avant barrière");
   wait_barrier(NB_PCS);
   puts("après barrière");
   exit(0);
}

int main(int argc, char* argv[]) {

	int NB_PCS;
	int i, pid;

	if(argc != 2) {
		printf("Bad args\n");
		return EXIT_FAILURE;
	}
	NB_PCS = atoi(argv[1]);

	if(!(barrier = sem_open("MaBar", O_CREAT|O_RDWR, 0600, 0)))
	{
		perror("semBar");
		return EXIT_FAILURE;
	}
	if(!(truc = sem_open("MonTruc", O_CREAT|O_RDWR, 0600, NB_PCS-1)))
	{
		perror("semTrucr");
		return EXIT_FAILURE;
	}

	for(i = 0; i < NB_PCS; i++) {
		if((pid = fork()) == -1) {
			perror("fork");
			return EXIT_FAILURE;
		}
		else if (pid == 0) {
			process(NB_PCS);
			/* process fait le exit */
		}
	}

	for(i = 0; i < NB_PCS; i++) {
		wait(NULL);
	}

	sem_close(truc);
	sem_close(barrier);
	sem_unlink("MaBar");
	sem_unlink("MonTruc");

	return EXIT_SUCCESS;
}