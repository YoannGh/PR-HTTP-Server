/*
4 Fonctions POSIX vs. fonctions C

Écrire en C deux fonctions qui lisent caractère par caractère un fichier passé en argument, et qui affichent chaque caractère dès qu’il est lu. Ces fonctions utiliseront 3 processus partageant le même descripteur et agissant à l’identique (l’identité de chaque processus est affichée avec le caractère lu). La première utilisera les standard POSIX open et read, la seconde fopen et fgetc. Vous écrirez un unique programme qui, selon que son premier argument est -p ou -C appliquera l’une ou l’autre de ces fonctions sur le fichier donné en 2e argument. Vous contrôlerez les cas d’erreur comme précédent. Quelle différence observez-vous entre les deux modes d’appels ?
Exemple d'appel :
$PWD/bin/lectures -p src/lectures.c; $PWD/bin/lectures -C src/lectures.c
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

#define NB_PROC 3


int print_char_posix(const char *path) 
{
	int fd, i, lus, p;
	char c;

	if ((fd = open (path, O_RDONLY, 0600)) == -1) {
      	perror("open fd");
      	return errno;
    }

    for (i = 0; i < NB_PROC; ++i)
	{
		if((p = fork()) == -1)
			perror("fork error");
		else if (p == 0) {
			lus = read(fd, &c, 1);
			while(lus != 0) {
				printf("%d lit le char: %c\n", getpid(), c);
				lus = read(fd, &c, 1);
			}
			return EXIT_SUCCESS;
		}
	}

	for (i = 0; i < NB_PROC; ++i)
	{
		wait(NULL);
	}

	close(fd);
	
	return EXIT_SUCCESS;
}

int print_char_c(const char *path) 
{
	FILE *fd;
	int i, p;
	int c;

	if ((fd = fopen(path, "r")) == NULL) {
      	perror("fopen fd");
      	return errno;
    }

    for (i = 0; i < NB_PROC; ++i)
	{
		if((p = fork()) == -1)
			perror("fork error");
		else if (p == 0) {
			c = fgetc(fd);
			while(c != EOF) {
				printf("%d lit le char: %c\n", getpid(), c);
				c = fgetc(fd);
			}
			return EXIT_SUCCESS;
		}
	}

	for (i = 0; i < NB_PROC; ++i)
	{
		wait(NULL);
	}

	fclose(fd);
	
	return EXIT_SUCCESS;
}

int main(int argc, char const **argv)
{
	if (argc != 3)
    {
        printf("usage: %s [-p|-C] filepath\n", argv[0]);
        return EXIT_FAILURE;
    }

    if(strcmp(argv[1], "-p") == 0)
    	return print_char_posix(argv[2]);
    else if (strcmp(argv[1], "-C") == 0)
    	return print_char_c(argv[2]);
	else {
		printf("Error: Unknown mode\nusage: %s [-p|-C] filepath\n", argv[0]);
		return EXIT_FAILURE;
	}
}