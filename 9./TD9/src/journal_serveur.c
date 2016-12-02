#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <linux/limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 4096

int init_server(int port, int maxclient) {
	struct sockaddr_in sin;
	int sock;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	listen(sock, maxclient);

	return sock;
}

void* processClient(void* arg)
{
	char * filename;
	int fdfile, nread, sock;
    char buffer[BUFFER_SIZE];

    filename = malloc(NAME_MAX);
    sock = *(int*)arg;
	memset(filename, 0, NAME_MAX);

	if((nread = recv(sock, filename, NAME_MAX, 0)) < 0) {
        perror("recv filename");
        goto clean;
    }

    printf("Receiving file: %s, nread: %d, NMAX: %d\n", filename, nread, NAME_MAX);

    if ((fdfile = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0600)) < 0) {
    	perror("open file");
    	goto clean;
	}

	printf("Created file: %s\n", filename);

	while((nread = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        if(write(fdfile, buffer, nread) < 0) {
            perror("write data");
            goto clean;
        }
	}

	printf("File %s transferred !\n", filename);

clean:
	close(fdfile);
	close(sock);
    free(arg);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    char buffer[INET6_ADDRSTRLEN];
	struct sockaddr_in exp;
	int sockclient, fd, i;
	socklen_t fromlen;
	int* sockTab;
	fd_set mselect;

	if(argc < 2) {
        printf("Bad args\n");
        return EXIT_FAILURE;
    }

    FD_ZERO(&mselect);

    sockTab = malloc(sizeof(int)*(argc-1));

    if ((fd = open("cx.log", O_CREAT|O_WRONLY, 0600)) < 0) {
    	perror("open file");
    	return errno;
	}

	for(i=1; i<argc; ++i)
    {
    	sockTab[i-1] = init_server(atoi(argv[i]), 120);
    	FD_SET(sockTab[i-1], &mselect);
    }

    while(1) {

    	puts("awaiting client");

		if(select(sockTab[i-2]+1, &mselect, NULL, NULL, NULL) == -1)
		{
			perror("select");
			return errno;
		}

    	puts("client connected");
    	for(i=0; i<argc-1; ++i)
    	{

    		if(FD_ISSET(sockTab[i], &mselect))
    		{
    			printf("Client: %d\n", sockTab[i]);
    			if((sockclient = accept(sockTab[i], (struct sockaddr *)&exp, &fromlen)) < 0) {
					perror("accept");
					return errno;
    			}

    			if(inet_ntop(AF_INET, &(exp.sin_addr), buffer, INET6_ADDRSTRLEN) == NULL)
    			{
					perror("inet_ntop");
					return errno;
    			}

				if(write(fd, buffer, INET6_ADDRSTRLEN) < 0) {
					perror("write data");
					return errno;
				}
				if(write(fd, "\n", 1) < 0) {
					perror("write data");
					return errno;
				}
			}
    	}
    	
    	FD_ZERO(&mselect);
        for(i=1; i<argc; ++i)
	    {
	    	FD_SET(sockTab[i-1], &mselect);
	    }
    }

    for(i=0; i<argc-1; ++i)
    	close(sockTab[i]);

    return EXIT_SUCCESS;


}