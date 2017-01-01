#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/select.h>
#include <netdb.h>
#include <string.h>

/* Pour test */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "client.h"
#include "util.h"
#include "http_request.h"

void client_init(client* client, http_server* server, int socket, char ip[INET_ADDRSTRLEN]);
{
	client->server = server;
	client->socket = socket;
	strncpy(client->ip, ip, INET_ADDRSTRLEN);
	
	client->prev_req_sem = (sem_t *) malloc(sizeof(sem_t));

	if(sem_init(client->prev_req_sem, 0, 1) < 0) {
		perror("failed to init first semaphore");	
	}
}

void client_destroy(client* client) { /* client must destroy itself */
	
	/* Fill later for DDOS */
	close(client->socket);
	sem_destroy(client->prev_req_sem);
	free(client->prev_req_sem);
	free(client); /* client must free itself in its thread to avoid memory leaks */
}

void* client_process_socket(void* arg) {
	client* client;
	pthread_t tid;
	fd_set readfds;
	struct timeval timeout = {30, 0}; /* wait request for 30 seconds */
	int res = 0;
	request* req;
	char** line;
	sem_t* new_req_sem;

	client = (client *) arg;
	if(set_nonblock(client->socket) < 0) {
#ifdef DEBUG
		puts("failed to make client socket non blocking");
#endif
	}

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
			if((res = readline(client->socket, &line)) < 0) {
#ifdef DEBUG
				puts("readline failed");
#endif				
				continue;
			}

			req = (request *) malloc(sizeof(request)); /* requests must free itself too */
			new_req_sem = (sem_t *) malloc(sizeof(sem_t));
			request_init(req, client, line, client->prev_req_sem, new_req_sem); /* saves line and init new_req_sem */
			client->prev_req_sem = new_req_sem;
			free(line);

			while(res > 0) {
				readline(client->socket, &line);
				res = strlen(line);
				free(line);
			}

			if(pthread_create(&tid, NULL, request_process, (void*) req) != 0) {
				perror("Thread processing request");
				client_destroy(client);
				return;
			}
		}

	}

	if (pthread_mutex_lock(&client->server->mutex_nbClient) < 0) {
		("lock mutex nbClient (in client)");
		/* Error: Do what? */
	}

	client->server->nbClient--;

	if(client->server->nbClient == (client->server->nbMaxClient - 1)) {
		/* Then server was waiting for a client to exit, wake it up */
		pthread_cond_signal(&client->server->cond_maxClient);
	}

	if (pthread_mutex_unlock(&client->server->mutex_nbClient) < 0) {
		perror("unlock mutex logger (in client)");
		/* Error: Do what? */
	}

	client_destroy(client);

	pthread_exit(NULL);

}

int main(void)
{
	client cl;
	int fd;
	fd = open("tmp.txt", O_RDWR| O_CREAT | O_TRUNC, 0600);
	client_init(&cl, fd, "127.0.0.1");
	client_process_socket(&cl);
}