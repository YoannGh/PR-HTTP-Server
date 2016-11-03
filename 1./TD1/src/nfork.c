/*
2 Création de processus

A l’aide de la fonction standard fork, définir la fonction
int nfork (int n)

qui crée n processus fils et renvoie :

    le nombre de processus fils créés, pour le processus père ;
    0 pour les processus fils ;
    -1 dans le cas où aucun fils n’a été créé.

Si la création d’un processus fils échoue, la fonction n’essaie pas de créer les autres fils. Dans ce cas, la fonction renvoie le nombre de fils qu’elle a pu créer (ou –1, si aucun fils n’a été créé). On pourra compléter cette fonction par une fonction main l’appliquant à un entier pas trop grand pour la tester.
Exemple d'appel :
$PWD/bin/nfork
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int nfork(int n) {
	int i;
	int ret;
	int cpt = 0;
	for(i = 0; i < n; i++) {
		ret = fork();
		if(ret == -1) {
			perror("Erreur fork");
			break;
		}
		else if(ret == 0) {
			//printf("fils : ");
			return 0;
		}
		else
			cpt++;
	}
	//printf("pere : ");
	if(cpt == 0)
		return -1;
	return cpt;
}
