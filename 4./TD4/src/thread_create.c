/*
1 Création de threads

Ecrire un programme prenant en argument un nombre N et créant N processus légers (à l’aide de pthread_create) et passant en paramètre à chacune son numéro de création compris entre 0 et N. Chacune affichera son numéro de création et son identité (utiliser pthread_self). Ensuite elle se termine, avec pthread_exit, en retournant son numéro de création multiplié par 2. De son côté, le programme principal doit attendre leur terminaison (à l’aide de pthread_join) en affichant la valeur renvoyée par chaque.
Exemple d'appel :
$PWD/bin/thread_create 10
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

void* affiche(void* arg) {
	int* num  = (int*) arg;
	printf("Num: %d et tid: %d\n", *num, (int)pthread_self());
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

	return EXIT_SUCCESS;
}