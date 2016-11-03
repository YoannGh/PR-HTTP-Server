/*
2 N fichiers , N-K Threads

On considère maintenant que le nombre de threads créées est inférieur au nombre de fichiers à traiter. Dès qu’une Thread a converti un fichier avec succès, elle doit passer à un autre fichier s’il en reste, et sinon se terminer. Si un fichier pose problème, elle se termine tout de suite en indiquant le fichier fautif au programme principal.

Ecrire un nouveau programme C programmant cette stratégie, le premier argument sur la ligne de commande étant le nombre de Thread permis, les suivants étant les fichiers, en nombre supérieur. Le programme principal doit attendre la fin des Threads, tester leur retour et afficher les fichiers ayant posé problème. On notera qu’il peut y avoir des fichiers non examinés si toutes les Threads ont rencontré un fichier à problème.
Exemple d'appel :
bin/thread_uppermax 2 fichier1 fichier2 fichier3
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "lib_upper.h"

int NB_FICHIER_TODO = 0;
pthread_mutex_t mutex_nb_fichier = PTHREAD_MUTEX_INITIALIZER;

void* upperDeleg(void* arg) {
	char** paths = (char **) arg;
	int *ret = malloc(sizeof(int));
	pthread_mutex_lock(&mutex_nb_fichier);
	while(NB_FICHIER_TODO > 0) {
		int filenumber = NB_FICHIER_TODO;
		NB_FICHIER_TODO--;
		pthread_mutex_unlock(&mutex_nb_fichier);
		*ret = upper(paths[filenumber+1]);
		if((*ret) != 0)
			pthread_exit(ret);
		pthread_mutex_lock(&mutex_nb_fichier);
	}
	pthread_mutex_unlock(&mutex_nb_fichier);

	pthread_exit(ret);
}

int main(int argc, char* argv[]) {

	pthread_t* tids;
	int nb_thread;
	int i, errorCount;
	int *threadret;

	errorCount = 0;

	if(argc < 3)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_thread = atoi(argv[1]);
	NB_FICHIER_TODO = argc -2;

	if( (nb_thread >= NB_FICHIER_TODO) || (nb_thread <= 0) )
	{
		printf("Wrong number of threads asked\n");
		return EXIT_FAILURE;
	}

	tids = malloc(sizeof(pthread_t)*(nb_thread));

	for(i = 0; i < (nb_thread); i++) 
	{
		if(pthread_create(&tids[i], NULL, upperDeleg, argv) != 0) {
			perror("error p_create");
			return EXIT_FAILURE;
		}
	}

	for(i = 0; i < nb_thread; i++) 
	{
		if((pthread_join(tids[i%nb_thread], (void*) &threadret) != 0) || !threadret) {
			printf("error p_join on file %s\n", argv[i+2]);
			errorCount++;
		}
	}

	free(tids);
	free(threadret);

	return errorCount;

}