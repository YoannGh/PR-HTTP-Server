/*
5 Attente de processus

Ecrire un programme qui crée deux processus fils fils1 et fils2. Chaque fils crée un fils, fils1.1 et fils2.1 respectivement. Ces 4 processus ne font qu’imprimer leur PID et PPID. Dans le cas du processus fils2, il imprime aussi le PID de son frère aîné fils1. Les processus n’ayant pas de fils se terminent aussitôt, mais un processus qui a des fils (y compris le processus principal) ne se termine qu’après ceux-ci. On utilisera la fonction wait pour réaliser cette attente, à l’exclusion de toute autre méthode (fichiers, fonction sleep etc).
Exemple d'appel :
$PWD/bin/mon_frere
*/

#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv) {
 	int p1, p2, p3= 0;

 	if((p1 = fork()) == -1) {
 		perror("Erreur fork principal");
 	} 

	
 	else if (p1 != 0) {
 		if((p2 = fork()) == -1)
 			perror("p2");

 		if((p1 != 0) && (p2 == 0))
 			printf("PID GRAND FRERE: %d\n", p1);

 	}
	if( p1 == 0 || p2 == 0)
	{
 		if((p3 = fork()) == -1)
 			perror("petitfils");

 		printf("PID: %d PPID:%d\n", getpid(), getppid());
	}

	if(p3 != 0)
		wait(NULL);

	if(p1 != 0 && p2 != 0)
		wait(NULL);



	return EXIT_SUCCESS;

}