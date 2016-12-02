#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <linux/limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>


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

	int port;
	struct sockaddr_in exp;
	int sock, sockclient;
	int* newsock;
	socklen_t fromlen;
	pthread_t threadid;

	if(argc != 2) {
        printf("Bad args\n");
        return EXIT_FAILURE;
    }

    port = atoi(argv[1]);


    sock = init_server(port, 120);

    while(1) {

    	puts("awaiting client");

		if((sockclient = accept(sock, (struct sockaddr *)&exp, &fromlen)) < 0) {
			perror("accept");
			return EXIT_FAILURE;
		}
		newsock = malloc(sizeof(int));
		*newsock = sockclient;
		puts("Client connected!");

		if(pthread_create(&threadid, NULL, processClient, (void*)newsock) < 0)
		{
			perror("eroor thread_create");
			return errno;
		}
    }

    close(sock);

    return EXIT_SUCCESS;


}