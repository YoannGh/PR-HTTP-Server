#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

int NB_WAIT_BARRIER = 0;
pthread_cond_t condBarriere = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void wait_barrier(int n)
{
	pthread_mutex_lock(&mutex);
	NB_WAIT_BARRIER++;
	if(NB_WAIT_BARRIER == n)
		pthread_cond_broadcast(&condBarriere);
	else
	{
		while(n > NB_WAIT_BARRIER)
		{
			pthread_cond_wait(&condBarriere, &mutex);
		}
	}
	pthread_mutex_unlock(&mutex);
}

void* thread_func (void *args) 
{
	printf ("avant barriere\n");
	wait_barrier (((int *)args)[0]);
   	printf ("après barriere\n");
   	pthread_exit ( NULL);
}	


int main(int argc, char* argv[]) {
	int i;
	int* nb_thread;
	pthread_t* tids;
	if(argc != 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_thread = malloc(sizeof(nb_thread));
	*nb_thread = atoi(argv[1]);
	tids = malloc(sizeof(pthread_t)*(*nb_thread));

	for(i = 0; i < (*nb_thread); i++) {
		if(pthread_create(&tids[i], NULL, thread_func, (void*) nb_thread) != 0) {
			perror("error p_create");
			return EXIT_FAILURE;
		}
	}

	for(i = 0; i < (*nb_thread); i++) {
		if(pthread_join(tids[i], NULL) != 0) {
			perror("error p_join");
			return EXIT_FAILURE;
		}
	}

	
	pthread_mutex_destroy(&mutex);

	return EXIT_SUCCESS;

}