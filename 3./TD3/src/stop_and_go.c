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
{}

int main(int argc, char const *argv[])
{
	struct sigaction  	action;
	sigset_t 			sig_proc, sig_set;
	int 			 	pidfils, children, i;

	if(argc != 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	children = atoi(argv[1]);

	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = SA_RESETHAND;
	action.sa_handler = sig_hand;

	sigemptyset(&sig_set);
	sigaddset(&sig_set, SIGCHLD);
	sigprocmask(SIG_SETMASK, &sig_set, NULL);

	for (i = 0; i < children ; ++i)
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
		printf("%d: Dernier fils s'auto stop\n", i);
		kill(getpid(),SIGSTOP);
		printf("%d: SIGCONT reçu\n", i);
	}
	else if(i != 0)
	{
		sigaction(SIGCHLD, &action, NULL);
		sigfillset(&sig_proc);
		sigdelset(&sig_proc, SIGCHLD);
		sigsuspend(&sig_proc);
		printf("%d: SIGCHLD reçu\n", i);
		kill(getpid(),SIGSTOP);
		printf("%d: SIGCONT reçu\n", i);
		kill(pidfils,SIGCONT);
	}
	else
	{
		sigaction(SIGCHLD, &action, NULL);
		sigfillset(&sig_proc);
		sigdelset(&sig_proc, SIGCHLD);
		sigsuspend(&sig_proc);
		printf("%d: SIGCHLD reçu\nTous les descendants sont suspendus\n", i);
		kill(pidfils,SIGCONT);
	}

	return EXIT_SUCCESS;
}