#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	int* pids;
	int nbProc, i, p, random;

	if (argc != 2)
	{
		printf("Error args\n");
		return EXIT_FAILURE;
	}

	nbProc = atoi(argv[1]);

	pids = malloc(nbProc+1);

	for(i = 0; i < nbProc+1; i++) {
		printf("%d\n", i);
		pids[i] = getpid();
		if((p = fork()) == -1) {
 			perror("Erreur fork");
 		}
 		else if(p != 0) {
 			if(i == 0)
 				printf("pere ici\n");
 			break;
 		}
	}

	if((i < nbProc+1) && (i != 0)) {
		printf("%d Termine: pid: %d, ppid: %d, fpid: %d\n", i, getpid(), getppid(), wait(&random));
		return random;
	}
	else if(i == 0) {
		printf("%d Termine: pid: %d, ppid: %d, fpid: %d\n", i, getpid(), getppid(), wait(&random));
		printf("Main proc: random: %d\n", random);
		return EXIT_SUCCESS;
	}

	printf("Dernier fils: pids:\n");
	for(i = 0; i < nbProc+1; i++)
		printf("%d ", pids[i]);
	printf("\n");


	srand(time(NULL));
	random = (int)(rand () /(((double) RAND_MAX +1) /100));
	printf("random dernier fils: %d\n", random);
	return random;
}