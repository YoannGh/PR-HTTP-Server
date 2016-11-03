/*
4 Des producteurs et des consommateurs

Reprenez l’exercice précédent en faisant échanger plusieurs producteurs et plusieurs consommateurs de façon concurrente, toujours à travers une seule pile. Votre programme prendra en argument deux nombres : le nombre de producteurs suivi du nombre de consommateurs.
Exemple d'appel :
echo "123456789" | bin/producteurs_consommateurs 5 3
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "producteur_consommateur.h"

#define MAX_SIZE 100

int stack_size = 0;
char stack[MAX_SIZE];
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_stackSize = PTHREAD_MUTEX_INITIALIZER;

void push(char c)
{
	pthread_mutex_lock(&mutex_stackSize);
	while(stack_size == MAX_SIZE)
	{
		pthread_cond_wait(&cond_full, &mutex_stackSize);
	}

	stack[stack_size] = c;
	stack_size++;
	if(stack_size == 1)
		pthread_cond_broadcast(&cond_empty);
	pthread_mutex_unlock(&mutex_stackSize);
}

char pop()
{
	char c;
	pthread_mutex_lock(&mutex_stackSize);
	while(stack_size == 0)
	{
		pthread_cond_wait(&cond_empty, &mutex_stackSize);
	}

	c = stack[stack_size -1];
	stack_size--;
	if(stack_size == MAX_SIZE-1)
		pthread_cond_broadcast(&cond_full);
	pthread_mutex_unlock(&mutex_stackSize);
	return c;
}



void* producteur(void * arg)
{
	PRODUCTEUR
	return arg;
}

void* consommateur(void * arg)
{
	CONSOMMATEUR
	return arg;
}




int main(int argc, char* argv[]) {

	pthread_t *tidProd, *tidCons;
	int nb_cons, nb_prod;
	int i;

	if(argc != 3)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_prod = atoi(argv[1]);
	nb_cons = atoi(argv[2]);

	if( nb_prod <= 0 || nb_cons <= 0 )
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	tidProd = malloc(sizeof(pthread_t)*nb_prod);
	tidCons = malloc(sizeof(pthread_t)*nb_cons);


	for(i=0;i<nb_cons;++i)
	{
		if(pthread_create(&tidCons[i], NULL, consommateur, NULL) != 0) {
				perror("error p_create cons");
				return EXIT_FAILURE;
		}
	}

	for(i=0;i<nb_prod;++i)
	{	
		if(pthread_create(&tidProd[i], NULL, producteur, NULL) != 0) {
				perror("error p_create prod");
				return EXIT_FAILURE;
		}
	}

	for(i=0;i<nb_prod;++i)
	{
		if(pthread_join(tidProd[i], NULL) != 0) {
			printf("error p_join prod");
			return EXIT_FAILURE;
		}
	}

	for(i=0;i<nb_cons;++i)
	{
		if(pthread_join(tidCons[i], NULL) != 0) {
			printf("error p_join cons");
			return EXIT_FAILURE;
		}
	}

	free(tidCons);
	free(tidProd);

	return EXIT_SUCCESS;

}