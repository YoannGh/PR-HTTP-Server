#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>


void sig_hand(int sig)
{
	if(sig == SIGUSR1)
		printf("P3 est crée\n");
	else if(sig == SIGUSR2)
		printf("P3 est mort\n");

}


int main()
{
	struct sigaction  	action;
	sigset_t 			sig_proc, sig_set;
	int 			 	pidGP, pidfils, i;

	pidGP = getpid();


	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;

	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGUSR1);
	sigaddset(&sig_set, SIGUSR2);
	sigaddset(&sig_set, SIGCHLD);
	sigprocmask(SIG_SETMASK, &sig_set, NULL);

	for (i = 0; i < 3 ; ++i)
	{
		if((pidfils = fork()) == -1)
		{
			perror("Fork failed");
			return errno;
		}
		if(pidfils != 0)
			break;
	}

	if(pidfils == 0)
	{
		kill(pidGP,SIGUSR1);

	}
	else if(i != 0)
	{
		sigaction(SIGCHLD, &action, NULL);
		sigfillset(&sig_proc);
		sigdelset(&sig_proc, SIGCHLD);
		sigsuspend(&sig_proc);
		kill(getppid(),SIGUSR2);
	}
	else
	{
		sigaction(SIGUSR1, &action, NULL);
		sigfillset(&sig_proc);
		sigdelset(&sig_proc, SIGUSR1);
		sigsuspend(&sig_proc);

		sigaction(SIGUSR2, &action, NULL);
		sigfillset(&sig_proc);
		sigdelset(&sig_proc, SIGUSR2);
		sigsuspend(&sig_proc);

		sigaction(SIGCHLD, &action, NULL);
		sigfillset(&sig_proc);
		sigdelset(&sig_proc, SIGCHLD);
		sigsuspend(&sig_proc);

		printf("P2 mort\n");
	}
	sigprocmask(SIG_UNBLOCK, &sig_set, NULL);

	return EXIT_SUCCESS;
}