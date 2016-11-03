/*
3 Un producteur et un consommateur

Nous voulons faire communiquer une thread Producteur et une thread Consommateur en utilisant une pile de taille fixe (un tableau de 100 caractères). Les valeurs empilées sont des caractères. La thread Producteur utilise la fonction Push() pour empiler un caractère au sommet de la pile et la thread Consommateur utilise la fonction Pop() pour désempiler une valeur du sommet de la pile. Une variable globale stack_size contrôle le sommet de la pile.

Programmez les fonctions Push() et Pop() décrites ci-dessus pour faire communiquer les threads Producteur et Consommateur, le corps de ces deux fonctions reposant respectivement sur les deux séquences de code définies par les deux macros suivantes, fournies dans le fichier .h en annexe :

#define PRODUCTEUR int c;  while((c = getchar()) != EOF){ push(c); }

#define CONSOMMATEUR while(1) { putchar(pop()); fflush(stdout); }

Ecrire ensuite le programme main utilisant ces deux fonctions.
Exemple d'appel :
echo "123456789" | bin/producteur_consommateur
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

	pthread_t tidProd, tidCons;
	int nb_prod, nb_cons;


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

	if(pthread_create(&tidCons, NULL, consommateur, NULL) != 0) {
			perror("error p_create cons");
			return EXIT_FAILURE;
	}

	if(pthread_create(&tidProd, NULL, producteur, NULL) != 0) {
			perror("error p_create prod");
			return EXIT_FAILURE;
	}

	if(pthread_join(tidProd, NULL) != 0) {
		printf("error p_join prod");
		return EXIT_FAILURE;
	}

	if(pthread_join(tidCons, NULL) != 0) {
		printf("error p_join cons");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}