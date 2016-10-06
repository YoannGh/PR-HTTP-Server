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

      pids = calloc(nbProc+1, sizeof(int));

      for(i = 0; i < nbProc; i++) {
            if((p = fork()) == -1) {
                  perror("Erreur fork");
            }
            /* printf("%d\n", i);*/
            pids[i] = getpid();
            if(p) {
                  printf("break\n");
                  break;
            }

      }

      if(i == nbProc) { /* dernier processus fils */
            printf("Dernier fils: pids:\n");
            for(i = 0; i < nbProc; i++)
                  printf("%d ", pids[i]);
            printf("\n");
            free(pids);

            srand(time(NULL));
            random = (int)(rand () /(((double) RAND_MAX +1) /100));
            printf("random dernier fils: %d, pid: %d\n", random, getpid());
            exit(random);
      }
      else {
            free(pids);
            p = wait(&random);
            printf("%d Termine: pid: %d, ppid: %d, fpid: %d\n", i, getpid(), getppid(), p);

            if(i == 0) { /* le processus main */
                  printf("Main proc: random: %d\n", random); 
                  return EXIT_SUCCESS;
            }
            else {
                  printf("%d termine avec rand: %d\n",i, random);
                  exit(random);
            }
      }

      return EXIT_SUCCESS;
}
