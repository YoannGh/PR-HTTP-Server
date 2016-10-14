#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

int GLOBAL = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* affiche(void* arg) {
	int* num  = (int*) arg;
	int random = (int) (10*((double)rand())/ RAND_MAX);
	printf("THREAD %d ; Random: %d\n", *num, random);
	pthread_mutex_lock(&mutex);
	GLOBAL+=random;
	pthread_mutex_unlock(&mutex);

	*num *= 2;
	pthread_exit(arg);
}

int main(int argc, char* argv[]) {
	int nb_thread;
	pthread_t* tids;
	int i;
	int* dup_i;
	if(argc != 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_thread = atoi(argv[1]);
	tids = malloc(sizeof(pthread_t)*nb_thread);

	for(i = 0; i < nb_thread; i++) {
		dup_i = malloc(sizeof(int));
		*dup_i = i;
		if(pthread_create(&tids[i], NULL, affiche, dup_i) != 0) {
			perror("error p_create");
			return EXIT_FAILURE;
		}
	}

	for(i = 0; i < nb_thread; i++) {
		if(pthread_join(tids[i], (void**) &dup_i) != 0) {
			perror("error p_join");
			return EXIT_FAILURE;
		}
		printf("Retour mult: %d\n", *((int*)(int*)dup_i));
	}

	printf("GLOBAL: %d\n", GLOBAL);
	pthread_mutex_destroy(&mutex);

	return EXIT_SUCCESS;

}