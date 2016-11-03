/*
1 N Threads pour N fichiers

On considère la fonction (on trouvera en annexe un fichier C la contenant que le Makefile fourni s’attend à trouver dans le sous-répertoire src) qui fait passer en majuscules toutes les minuscules d’un fichier :

#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

int upper(char *path)
{
    FILE* fp1, *fp2;
    int c = 1;

    fp1= fopen (path, "r");
    fp2= fopen (path, "r+");
    if ((fp1== NULL) || (fp2== NULL)) {
        perror (path);
	return errno;
    }

    while (c !=EOF) {
        c=fgetc(fp1);
        if (c!=EOF)
        fputc(toupper(c),fp2);
    }

    fclose (fp1);
    fclose (fp2);

    return EXIT_SUCCESS;
}

Ecrire un programme C prenant plusieurs noms de fichiers en argument. Il doit créer autant de Threads que de fichiers, et les lancer en parallèle. La i-ème Thread créée doit appliquer la fonction ci-dessus sur le i-ème fichier de la liste des fichiers, et transmettre au programme principal le résultat de cette fonction. Le programme principal attend la terminaison de chaque Thread et teste son retour. S’il n’est pas nul, il affiche le nom du fichier posant problème sur le flux de sortie. Au final, le programme sort avec comme code de retour le nombre de fichiers qui ont posé problème (donc 0 si tout c’est bien passé).
Exemple d'appel :
bin/thread_upper fichier1 fichier2 fichier3 fichier4
*/

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "lib_upper.h"

void* upperDeleg(void* arg) {
	char* path = (char *) arg;
	int *ret = malloc(sizeof(int));
	*ret = upper(path);
	pthread_exit(ret);
}

int main(int argc, char* argv[]) {

	pthread_t* tids;
	int nb_thread;
	int i, errorCount;
	int *threadret;
	char* arg;

	errorCount = 0;

	if(argc < 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_thread = argc -1;
	tids = malloc(sizeof(pthread_t)*(nb_thread));

	for(i = 0; i < (nb_thread); i++) {

		arg = argv[i+1];
		if(pthread_create(&tids[i], NULL, upperDeleg, arg) != 0) {
			perror("error p_create");
			return EXIT_FAILURE;
		}
	}

	for(i = 0; i < (nb_thread); i++) {
		if((pthread_join(tids[i], (void**) &threadret) != 0) || !threadret) {
			printf("error p_join on file %s", argv[i+1]);
			errorCount++;
		}
	}

	free(tids);
	free(threadret);

	return errorCount;

}