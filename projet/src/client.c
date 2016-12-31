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
#include <sys/select.h>


void client_init(client* client, http_server* server, int socket, char ip[INET_ADDRSTRLEN]);
{
	client->server = server;
	client->socket = socket;
	strncpy(client->ip, ip, INET_ADDRSTRLEN);
}

void client_destroy(client* client) /* client must destroy itself */
{
	/* Fill later for DDOS */
	close(client->socket);
	free(client); /* client must free itself in its thread to avoid memory leaks */
}

void client_process_socket(client* client)
{
	pthread_t tid;
	fd_set readfds;
	struct timeval timeout = {30, 0}; /* wait request for 30 seconds */
	int res = 0;
	request* req;
	char** line;

	FD_ZERO(&readfds);
	FD_SET(client->socket, &readfds);

	while(1) {

		if((res = select(client->socket+1, &readfds, NULL, NULL, &timeout)) < 0) {
			perror("select");
			/* Do what ? */
		}

		if(res == 0) /* timeout expired */
			break;

		else if(FD_ISSET(client->socket, &readfds)) { /* request received */
			/* TODO: Decouper les requetes + lancer thread requete */
			socket_getline(client->socket, line);
			req = (request *) malloc(sizeof(request)); /* requests must free itself too */
			request_init(req, client, line);

			if(pthread_create(&tid, NULL, request_process, (void*) req) != 0) {
				perror("Thread processing request");
				client_destroy(client);
				return;
			}
		}

	}

	client_destroy(client);

}

int main(void)
{
	client cl;
	int fd;
	fd = open("tmp.txt", O_RDWR| O_CREAT | O_TRUNC, 0600);
	client_init(&cl, fd, "127.0.0.1");
	client_process_socket(&cl);
}