/*
1 Tube et majuscules

On souhaite écrire un programme qui mette en majuscules les chaînes de caractères entrées par l’utilisateur via le terminal. Ce programme lance 2 processus :

    le processus père créé un tube avec la fonction C pipe et un processus fils, récupère les messages utilisateur en les lisant sur l’entrée standard, puis les transmet à son fils ;
    le processus fils lit les messages de son père, les transcrit en majuscules avec la fonction C toupper, puis les écrit sur la sortie standard.

Exemple d'appel :
echo abcd.ext | $PWD/bin/pipe_maj
*/

#define _POSIX_SOURCE 1
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#define S_BUF 100
#define ECRITURE 1
#define LECTURE 0

int main()
{
      int tubeDesc[2];
      char buffer[S_BUF+1];
      char c;
      pid_t pid;
      int n, i;

      if(pipe (tubeDesc) == -1)
      {
            perror ("pipe");
            exit(1);
      }

      n = 0;
      if((pid = fork()) == -1)
      {
            perror("fork");
            exit(2);
      }

      if(pid == 0)
      {
            if(( n = read(tubeDesc[LECTURE], buffer, S_BUF)) == -1)
            {
                  perror ("read");
                  exit(3);
            }
            buffer[n] = '\0';
            i = 0;
            while (buffer[i])
            {
                  c = buffer[i];
                  putchar (toupper(c));
                  ++i;
            }
            close(tubeDesc[0]);
      }
      else
      {
            if(( n = write(tubeDesc[ECRITURE], buffer, read(0, buffer, S_BUF))) == -1)
            {
                  perror ("write");
                  exit(4);
            }
            close(tubeDesc[1]);
      }

      return EXIT_SUCCESS;
}
