/*
3 Attente ordonnée de processus

On reprend l’énoncé de l’exercice Attente de processus où un processus crée deux fils créant chacun un processus. On ajoute la contrainte que le processus fils 1 ne peut se terminer qu’après les affichages réalisés par son frère fils 2 et son fils fils 1.1. Comme auparavant, les processus ne se terminent qu’après envoi des messages de leur fils respectifs. Trouvez une solution n’utilisant que les signaux SIGUSR1 et SIGUSR2, à l’exclusion de tout autre moyen (Wait, Sleep etc).
Exemple d'appel :
$PWD/bin/mon_petit_frere
*/

#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void sig_hand(int sig)
{

}

int main() {
 	int p1, p2, p3= 0;

 	struct sigaction  	action;
	sigset_t 			sig_proc, sig_set;

	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;

	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGUSR1);
	sigaddset(&sig_set, SIGUSR2);
	sigprocmask(SIG_SETMASK, &sig_set, NULL);

 	if((p1 = fork()) == -1) {
 		perror("Erreur fork principal");
 	} 
	
 	else if (p1 != 0) {
 		if((p2 = fork()) == -1)
 			perror("p2");

 	}

	if( p1 == 0 || p2 == 0) { /* si on est un fils du proc main */
 		if((p3 = fork()) == -1)
 			perror("petitfils");

 		printf("PID: %d PPID:%d\n", getpid(), getppid());

 		if((p1 != 0) && (p2 == 0)) { /* P2 */
 			printf("PID GRAND FRERE: %d\n", p1);
 			kill(p1, SIGUSR2);
 		}

 		if(p3 == 0) {
 			if(p1 == 0) { /* fils 1.1*/
 				kill(getppid(), SIGUSR1);
 			}
 			else if (p2 == 0) /* fils 2.1*/
 			{
 				kill(getppid(), SIGUSR2);
 			}
 		}
	}

	if(p3 != 0) {
		if(p1 == 0) { /* fils 1*/
			sigaction(SIGUSR1, &action, NULL);
			sigfillset(&sig_proc);
			sigdelset(&sig_proc, SIGUSR1);
			sigdelset(&sig_proc, SIGUSR2);
			sigsuspend(&sig_proc);
			sigsuspend(&sig_proc);
			kill(getppid(), SIGUSR1);
		}
		else if(p2 == 0) { /* fils 2*/
			sigaction(SIGUSR2, &action, NULL);
			sigfillset(&sig_proc);
			sigdelset(&sig_proc, SIGUSR2);
			sigsuspend(&sig_proc);
			kill(getppid(), SIGUSR2);
		}
	}

	if(p1 != 0 && p2 != 0) { /* si proc main*/
		sigaction(SIGUSR1, &action, NULL);
		sigfillset(&sig_proc);
		sigdelset(&sig_proc, SIGUSR1);
		sigdelset(&sig_proc, SIGUSR2);
		sigsuspend(&sig_proc);
		sigsuspend(&sig_proc);
	}

	return EXIT_SUCCESS;
}