#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#define S_BUF 100

int keep_reading;

void sig_hand(int sig)
{
	printf("CTRL C FTW BB\n");
}

int main(int argc, char **argv)
{

	struct sigaction  	action;
	int tube[2], fd_read, n;
	char buffer[S_BUF];
	char* cpt;

	keep_reading = 1;

	if(argc != 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	if((fd_read = open(argv[1], O_RDONLY|O_NONBLOCK) == -1))
	{
		perror("read");
		exit(1);
	}

	action.sa_handler = sig_hand;
	sigaction(SIGINT, &action, NULL);

	while(keep_reading)
	{
		if((n = read(fd_read, buffer, S_BUF) == -1))
		{
			perror("read");
			exit(1);
		}
		buffer[n] = '\0';
		cpt = buffer;
		while(*cpt)
			printf("%c", *cpt++);
	}

	close(fd_read);

	return EXIT_SUCCESS;
}