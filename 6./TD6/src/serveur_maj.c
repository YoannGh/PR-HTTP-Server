#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

#define S_BUF 100

int keep_reading;

void sig_hand(int sig)
{
	sig++;
	keep_reading = 0;
}

int main(int argc, char **argv)
{

	struct sigaction  	action;
	int fd_read, n;
	char buffer[S_BUF];
	char* cpt;

	keep_reading = 1;

	if(argc != 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	if(mkfifo(argv[1], S_IRUSR|S_IWUSR) != 0) {
		perror("error mkfifo\n");
		return EXIT_FAILURE;
	}

	if((fd_read = open(argv[1], O_RDONLY)) == -1)
	{
		perror("read");
		exit(1);
	}

	action.sa_handler = sig_hand;
	sigaction(SIGINT, &action, NULL);


	while(keep_reading)
	{
		if((n = read(fd_read, buffer, S_BUF)) == -1)
		{
			perror("read");
			exit(1);
		}
		buffer[n] = '\0';
		cpt = buffer;
		while(*cpt)
			putchar(toupper(*cpt++));

	}

	close(fd_read);
	unlink(argv[1]);

	return EXIT_SUCCESS;
}