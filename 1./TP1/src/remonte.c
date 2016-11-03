/*
2 Remontée de valeurs

Ecrire en C un programme prenant en argument un nombre n et un nom de fichier. Le processus principal doit créer n processus fils, à l’aide de fork. Chaque processus fils produit une valeur aléatoire qu’il insère dans le fichier donné en 2e argument, à destination du processus principal. La valeur aléatoire est calculée par :

(int) (10*(float)rand()/ RAND_MAX)

De son côté, le processus principal doit attendre la terminaison de tous ses fils, puis extraire toutes les valeurs du fichier pour ensuite les additionner et enfin afficher la somme résultante. On pourra utiliser waitpid appliqué aux résultat de fork pour attendre les terminaisons, puis utiliser lseek. On contrôlera les cas d’erreurs comme à l’exercice précédent.
Exemple d'appel :
$PWD/bin/remonte 8 aleas
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char const **argv)
{
	int fd, i, p, random, lus = -1, sum = 0;
	int nbfils = atoi(argv[1]);
	char c[4];

 	if (argc != 3)
    {
        printf("Specifiez deux args svp\n");
        return EXIT_FAILURE;
    }

    if ((fd = open (argv[2], O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1) {
      	perror("open fd");
      	return errno;
    }

	


	for(i = 0; i < nbfils; i++) {
		
		if((p = fork()) == -1)
			perror("fork error");
		else if (p == 0) {
			random = ((int) (10*(float)rand()/ RAND_MAX));
			sprintf(c, "%d", random);
			write(fd, &c, sizeof(char));
			break;
		}
		else
			srand(p);
	}
	if(p != 0)
	{
		for(i = 0; i < nbfils; i++) {
			wait(NULL);
		}
	
		lseek(fd, 0, SEEK_SET);
	
		lus = read(fd, &c, 1);
		while(lus != 0) {
			sum += atoi(c);
			lus = read(fd, &c, 1);

		}
	
		printf("SOMME: %d\n", sum);
	}

	close(fd);

	return 0;
}

// srand (time (NULL));
// 	int n = (int) (10*(float)rand()/ RAND_MAX);
// 	printf("%d\n", n);
