#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#define S_BUF 100

int keep_reading;

void sig_hand(int sig)
{
	printf("CTRL C FTW BB %d\n", sig);
	keep_reading = 0;
}

int main(int argc, char **argv)
{

	struct sigaction  	action;
	int fd_read, n;
	char buffer[S_BUF];
	char* cpt;
	struct stat stat;

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

	if((fd_read = open(argv[1], O_RDONLY/*|O_NONBLOCK*/) == -1))
	{
		perror("read");
		exit(1);
	}

	action.sa_handler = sig_hand;
	sigaction(SIGINT, &action, NULL);

	if(fstat(fd_read, &stat) == -1) {
		perror("fstat");
	}
	
	if(S_ISFIFO(stat.st_mode)) {
		puts("fifo");	
	}
	if(S_ISBLK(stat.st_mode)) {
		puts("blk");
	}
	if(S_ISCHR(stat.st_mode)) {
		puts("chr");
	}
	if(S_ISDIR(stat.st_mode)) {
		puts("dir");
	}
	if(S_ISREG(stat.st_mode)) {
		puts("reg");
	}

		printf("s numinode: %d\n", (int) stat.st_ino);
		printf("s nblien: %d\n", (int) stat.st_nlink);

	while(keep_reading)
	{
		puts("server: avant read");
		if((n = read(fd_read, buffer, S_BUF) == -1))
		{
			perror("read");
			exit(1);
		}
		puts("server: truc lu");
		buffer[n] = '\0';
		cpt = buffer;
		while(*cpt)
			printf("%c", *cpt++);
	}

	close(fd_read);
	unlink(argv[1]);

	return EXIT_SUCCESS;
}