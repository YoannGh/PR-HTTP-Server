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
	printf("SIGUSR1 ok, pid: %d\n", getpid());
}

int main(int argc, char const *argv[])
{
	struct sigaction  	action;
	sigset_t 			sig_proc;
	int 			 	pid, children, i;
	int* 				pids;

	if(argc != 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	children = atoi(argv[1]);
	pids = malloc(children+1);

	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;


	for (i = 0; i < children ; ++i)
	{
		pids[i] = getpid();
		if((pid = fork()) == -1)
		{
			perror("Fork failed");
			return errno;
		}
		if(pid != 0)
		{
			sigaction(SIGUSR1, &action, NULL);
			pids[i+1] = pid;
			break;
		}
	}

	pids[i] = getpid();

	if(pid == 0)
	{
		printf("Liste des pids: ");
		for(i = 0; i < children+1; i++)
			printf("%d ", pids[i]);
		printf("\n");
		kill(getppid(),SIGUSR1);
	}
	else
	{
		sigfillset(&sig_proc);
		sigdelset(&sig_proc, SIGUSR1);
		sigsuspend(&sig_proc);
		if( i != 0)
			kill(getppid(),SIGUSR1);
		printf("Fin de: %d, ppid: %d, fpid: %d\n", getpid(), getppid(), pids[i+1]);
	}

	return EXIT_SUCCESS;
}