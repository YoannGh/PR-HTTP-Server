#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int sock, nb_proc, pid, random, i;
	struct sockaddr_un addr;	
	
	if(argc != 3) {
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_proc = atoi(argv[2]);

	memset(&addr, '\0', sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, argv[1]);

	if((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	if(bind(sock, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
		perror("bind");
		return EXIT_FAILURE;
	}

	for(i = 0; i < nb_proc; i++) {
    	pid = fork();
    	if (pid == -1){
      		perror("fork");
      		return EXIT_FAILURE;
    	} else if (pid == 0) {
      		srand(time(NULL)*i);
      		random = (int) (10*(float)rand()/ RAND_MAX);
      		sendto(sock, (const void*) &random, sizeof(int), 0, (const struct sockaddr *)&addr, sizeof(addr));
      		exit(EXIT_SUCCESS);
    	}
  	}

  	for(i = 0; i < nb_proc; i++) {
  		recvfrom(sock, (void *) &random, sizeof(int), 0, NULL, 0);
  		printf("Random received: %d\n", random);
  	}

  	for(i = 0; i < nb_proc; i++) {
    	wait(NULL);
  	}

  	close(sock);
  	unlink(argv[1]);

  	return EXIT_SUCCESS;

}