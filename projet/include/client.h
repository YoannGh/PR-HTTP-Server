#ifndef CLIENT_H_  
#define CLIENT_H_

#include <netdb.h>

typedef struct client {
	http_server* server;
	/*struct sockaddr_in* client_addr;*/
	int socket;
	char ip[INET_ADDRSTRLEN];

} client;

void client_init(client* client, http_server* server, int socket, char ip[INET_ADDRSTRLEN]);

void client_process_socket(client* client);

void client_destroy(client* client);

#endif 