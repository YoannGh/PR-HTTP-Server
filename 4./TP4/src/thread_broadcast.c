/*
3 Synchronisation par broadcast

Une barrière est un mécanisme de synchronisation. Elle permet à N threads de prendre rendez-vous en un point donné de leur exécution. Dès que l’une d’entre elles atteint la barrière, elle reste bloquée jusqu’à ce que toutes les autres y arrivent. Lorsque toutes sont arrivées, chacune peut alors reprendre son exécution.

Ecrire une fonction, qu’on nommera wait_barrier prenant en argument un entier N, permettant à N threads de se synchroniser sur une barrière. Testez votre programme avec la thread suivante :
void* thread_func (void *arg) {
   printf ("avant barriere\n");
   wait_barrier (((int *)args)[0]);
   printf ("après barriere\n");
   pthread_exit ( NULL);
}

En exécutant votre programme avec 2 threads, il devra afficher :

avant barrière
avant barrière
après barrière
après barrière

En d’autres termes, on veut que tous les messages « avant barrière » soient affichés avant les messages « après barrière ».
Exemple d'appel :
$PWD/bin/thread_broadcast 10
*/

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