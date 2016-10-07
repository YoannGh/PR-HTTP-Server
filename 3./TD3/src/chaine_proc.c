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
