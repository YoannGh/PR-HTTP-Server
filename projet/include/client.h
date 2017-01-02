#ifndef CLIENT_H_
#define CLIENT_H_

#include <semaphore.h>
#include <netinet/in.h> /* for INET_ADDRSTRLEN macro */

#include "http_server.h"

typedef struct client {
	http_server* server;
	int socket;
	char ip[INET_ADDRSTRLEN];
	sem_t* prev_req_sem;
} client;

void client_init(client* client, http_server* server, int socket, char ip[INET_ADDRSTRLEN]);

void* client_process_socket(void* client);

void client_destroy(client* client);

#endif 