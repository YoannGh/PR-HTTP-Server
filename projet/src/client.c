#include <client.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <http_request.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




void client_init(client* client, int socket, char ip[INET_ADDRSTRLEN])
{
	client->socket = socket;
	strncpy(client->ip, ip, INET_ADDRSTRLEN);
}

void client_destroy(client* client)
{
	/* Fill later for DDOS */
}

void client_process_socket(client* client)
{
	pthread_t tid;
	/*Decouper les requetes*/
	/*Pour chaque requete trouvée: */

	/* TODO init les semaphores dans struct request + logs */
	request* req = malloc(sizeof(request));
	request_init(req, client, "GET /dsqd.html HTTP/1.1\nHost: 127.0.0.1");

	if(pthread_create(&tid, NULL, request_process, (void*) req) != 0) {
			perror("Thread processing request");
			return;
	}

	pthread_join(tid, NULL);
}

int main(void)
{
	client cl;
	int fd;
	fd = open("tmp.txt", O_RDWR| O_CREAT | O_TRUNC, 0600);
	client_init(&cl, fd, "127.0.0.1");
	client_process_socket(&cl);
}