/*
1 Recopie de fichier

Ecrire en C un programme qui prend en argument deux noms de fichier et recopie intégralement le contenu du premier dans le second, en utilisant les fonctions POSIX open, read et write. On donnera au deuxième fichier les droits en lecture et écriture pour soi (à l’aide du 3e argument de open ou en appelant la fonction chmod). On veillera à dénoncer (avec la fonction perror) les cas d’erreur suivants :

    la ligne de commande ne contient pas exactement 2 noms ;
    le premier nom ne désigne pas un fichier régulier et accessible en lecture ;
    le second ne peut être créé (répertoire inaccessible en écriture, ou entrée déjà existante dedans).

En cas de réussiste, le programme 0 sinon il retourne la valeur de errno.
Exemple d'appel :
$PWD/bin/mycp src/mycp.c cp.c
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

int main (int argc, char ** argv) {

    int SIZE_TAMPON = 6;
    int fd1, fd2, n = -1;
    char tampon[SIZE_TAMPON];
    if(argc != 3)
    {
        printf("Specifiez deux noms de fichier svp\n");
        return EXIT_FAILURE;
    }

    if ((fd1 = open (argv[1], O_RDONLY)) == -1) {
      perror("fd1");
      return errno;
    }

    if(access(argv[2], O_RDWR) == 0)
    {
        printf("Le fichier dest existe déjà !\n");
        return EXIT_FAILURE;
    }

    if ((fd2 = open (argv[2], O_RDWR|O_CREAT, 0600)) == -1) {
      perror("open fd2");
      return errno;
    }

    while(n != 0) 
    {
          if((n=read(fd1,&tampon, SIZE_TAMPON)) == -1)
          {
              perror("read fd1");
              return errno;
          }

          printf("Char lus:%d\n", n);

          if ((n = write (fd2,&tampon,n)) == -1) {
            perror("write fd2");
            return errno;
          }

          printf("Char ecris:%d\n", n);
    }

    if(close(fd1) == -1)
    {
        perror("close1");
        return errno;
    }

    if(close(fd2) == -1)
    {
        perror("close2");
        return errno;
    }

    return EXIT_SUCCESS;
}