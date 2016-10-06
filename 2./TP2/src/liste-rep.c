#define _XOPEN_SOUCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

extern int errno;

#define _POSIX_SOUCE 1
#define TAILLE_PATH 100

char buff_path [TAILLE_PATH];
DIR *pt_Dir;
struct dirent* dirEnt;

int main (int argc, char* argv []) {
	
	if (argc ==1 ) {
		/* repertoir courant : obtenir le nom */
		if (getcwd (buff_path, TAILLE_PATH) == NULL ) {
			perror ("erreur getwcd \n");
			exit (1);
		}
	}
	else
		memcpy (buff_path,argv[1],strlen(argv[1]));
	
	if ( ( pt_Dir = opendir (buff_path) ) == NULL) {
		if (errno ==  ENOENT ) {
			/* repertoire n'existe pas - créer le répertoire */
			if (mkdir ( buff_path, S_IRUSR|S_IWUSR|S_IXUSR) == -1) {
				perror ("erreur mkdir\n");
				exit (1);
			}
			else
				return 0;
		}
		else {
			perror ("erreur opendir \n");
			exit (1);
		}
	}
	
	/* lire répertoire */
	while ((dirEnt= readdir (pt_Dir)) !=NULL) {
		printf ("%s\n", dirEnt->d_name);
	}
	closedir (pt_Dir);
	
	return 0;
}
