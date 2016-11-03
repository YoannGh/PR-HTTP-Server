/*
3 Réveil de threads

Modifier le programme précédent pour que la valeur finale soit affichée non plus par le programme principal mais par une nouvelle thread créée au départ. Celle-ci, après sa création, doit se bloquer en attendant que la somme de toutes les valeurs aléatoires soit complétée. La dernière thread à ajouter sa valeur aléatoire utilisera pthread_cond_signal pour signifier à la première qu’elle peut afficher la valeur de la globale. On utilisera une variable statique dans la fonction appelée à la création de la thread pour compter le nombre de ses appels, et repérer ainsi le dernier appel (le problème peut se résoudre sans d’autres variables globales que celle additionnant les valeurs aléatoires).
Exemple d'appel :
$PWD/bin/thread_wait 10
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

int GLOBAL = 0;
int NB_AFFICHAGE = 0;
int NB_THREAD;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condFinAffichage = PTHREAD_COND_INITIALIZER;

void* attendre(void* arg) {
	pthread_mutex_lock(&mutex);
	while(NB_AFFICHAGE < NB_THREAD)
		pthread_cond_wait(&condFinAffichage, &mutex);
	printf("GLOBAL: %d\n", GLOBAL);
	pthread_mutex_unlock(&mutex);
	pthread_exit(arg);
}


void* affiche(void* arg) {
	int* num  = (int*) arg;
	int random = (int) (10*((double)rand())/ RAND_MAX);
	printf("THREAD %d ; Random: %d\n", *num, random);
	*num *= 2;
	pthread_mutex_lock(&mutex);
	GLOBAL+=random;
	NB_AFFICHAGE++;
	if(NB_AFFICHAGE == NB_THREAD)
		pthread_cond_signal(&condFinAffichage);
	pthread_mutex_unlock(&mutex);
	
	pthread_exit(arg);
}

int main(int argc, char* argv[]) {
	pthread_t* tids;
	pthread_t afficheurDeFin;
	int i;
	int* dup_i;
	if(argc != 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	NB_THREAD = atoi(argv[1]);
	tids = malloc(sizeof(pthread_t)*NB_THREAD);

	if(pthread_create(&afficheurDeFin, NULL, attendre, NULL) != 0) {
			perror("error p_create");
			return EXIT_FAILURE;
	}

	for(i = 0; i < NB_THREAD; i++) {
		dup_i = malloc(sizeof(int));
		*dup_i = i;
		if(pthread_create(&tids[i], NULL, affiche, dup_i) != 0) {
			perror("error p_create");
			return EXIT_FAILURE;
		}
	}

	for(i = 0; i < NB_THREAD; i++) {
		if(pthread_join(tids[i], (void**) &dup_i) != 0) {
			perror("error p_join");
			return EXIT_FAILURE;
		}
		printf("Retour mult: %d\n", *((int*)(int*)dup_i));
	}

	if(pthread_join(afficheurDeFin, NULL) != 0) {
		perror("error p_join");
		return EXIT_FAILURE;
	}

	
	pthread_mutex_destroy(&mutex);

	return EXIT_SUCCESS;

}