/*
4 Redirection

En utlisant la fonction dup2, écrire une fonction Rediriger_stdout redirigeant la sortie standard vers un fichier donné en argument. On considère que le fichier n’existe pas. Ecrire ensuite une deuxième fonction Restaurer_stdout qui restaure la sortie vers le terminal. Ecrire enfin une fonction main prenant en argument un nom de fichier, et qui appelle 3 fois la fonction printf, les deux premiers appels encadrant un appel à Rediriger_stdout sur le fichier indiqué, et les deux derniers un appel à Restaurer_stdout. Qu’observez-vous dans le flux de sortie et le fichier ?
Exemple d'appel :
$PWD/bin/rediriger trace.txt
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
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

int Rediriger_stdout(int fd)
{
	return dup2(fd,STDOUT_FILENO);
}

int Restaurer_stdout(int fd)
{
	return dup2(fd, STDOUT_FILENO);
}


int main(int argc, char const *argv[])
{
	int fd, fdsave;
	fdsave = dup(STDOUT_FILENO);

	if(argc != 2) {
		printf("args\n");
	    return EXIT_FAILURE;
	}

	if ((fd = open (argv[1], O_RDWR|O_CREAT|O_TRUNC, 0600)) == -1) {
      perror("open fd");
      return errno;
    }

    printf("Avant_redirection\n");
    Rediriger_stdout(fd);
    printf("Apres_redirection\n");
    printf("Avant_restauration\n");
    Restaurer_stdout(fdsave);
    printf("Apres_restauration\n");

    close(fd);

	return EXIT_SUCCESS;
}