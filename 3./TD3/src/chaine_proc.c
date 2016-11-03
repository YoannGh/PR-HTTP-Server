/*
1 Chaîne de processus

A l’aide de la fonction fork, ecrire un programme qui crée une chaîne de processus telle que le processus initial (celui du main) crée un processus qui à son tour en crée un second et ainsi de suite jusqu’à la création de N processus (en plus du processus initial). Au moment de sa création, le dernier processus de la chaîne affiche le Pid de tous les autres processus y compris celui du processus initial. Chacun des autres processus attend la terminaison de son fils, puis affiche son propre Pid (à l’aide de getpid), celui de son père (à l’aide de getppid) et celui de son fils avant de se terminer.

On souhaite de plus que le dernier processus créé génère une valeur aléatoire entre 0 et 100. Pour générer cette valeur aléatoire utilisez :

(int)(rand () /(((double) RAND_MAX +1) /100))

Ecrire le programme de sorte que le processus initial affiche cette valeur aléatoire avant de se terminer
Exemple d'appel :
$PWD/bin/chaine_proc 10
*/

#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

int main(int argc, char const *argv[])
{
      int* pids;
      int nbProc, i, p, random;

      if (argc != 2)
      {
            printf("Error args\n");
            return EXIT_FAILURE;
      }

      nbProc = atoi(argv[1]);

      pids = calloc(nbProc, sizeof(int));

      for(i = 0; i < nbProc; i++) {
      		pids[i] = getpid();
            if((p = fork()) == -1) {
                  perror("Erreur fork");
            }
            if(p) {
                  break;
            }

      }

      if(i == nbProc) { /* dernier processus fils */
            printf("Dernier fils: pids:\n");
            for(i = 0; i < nbProc; i++)
                  printf("%d ", pids[i]);
            printf("\n");
            free(pids);
            random = (int)(rand () /(((double) RAND_MAX +1) /100));
            printf("random creer par dernier fils: %d\n", random);
            exit(random);
      }
      else {
            free(pids);
            p = wait(&random);
            random = WEXITSTATUS(random);
            printf("%d Termine: pid: %d, ppid: %d, fpid: %d\n", i, getpid(), getppid(), p);

            if(i == 0) { /* le processus main */
                  printf("random recu par main: %d\n", random); 
                  return EXIT_SUCCESS;
            }
            else {
                  exit(random);
            }
      }

      return EXIT_SUCCESS;
}
