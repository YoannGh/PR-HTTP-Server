#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include "http_server.h"
#include "client.h"

static int init_server(int port, int maxclient) {
	struct sockaddr_in sin;
	int sock;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("bind");
		return -1;
	}

	listen(sock, maxclient);

	return sock;
}

void http_server_init(http_server* server, int numPort, int nbMaxClient, antidos *antiDOS) {
	
	server->log = (logger *) malloc(sizeof(logger));
	logger_init(server->log);
	server->numPort = numPort;
	server->nbMaxClient = nbMaxClient;
	server->antiDOS = antiDOS;
	server->nbClient = 0;
	pthread_mutex_init(&server->mutex_nbClient, NULL);
	pthread_cond_init(&server->cond_maxClient, NULL);

#ifdef DEBUG
	puts("HTTP Server initialized");
#endif

}

void http_server_destroy(http_server* server) {
	
	logger_destroy(server->log);
	free(server->log);
	pthread_mutex_destroy(&server->mutex_nbClient);
	pthread_cond_destroy(&server->cond_maxClient);

#ifdef DEBUG
	puts("HTTP Server destroyed");
#endif
}

void http_server_run_loop(http_server* server) {

	struct sockaddr_in exp;
	char ip[INET_ADDRSTRLEN];
	int socket_server, socket_client;
	socklen_t fromlen = sizeof exp;
	client* cl;
	pthread_t thread;

	if ((socket_server = init_server(server->numPort, server->nbMaxClient)) < 0) {
#ifdef DEBUG
		puts("error in init_server");
#endif
		exit(EXIT_FAILURE);
	}

#ifdef DEBUG
		puts("Server running ... Starting to accept clients");
#endif

	while(1) {

		if((socket_client = accept(socket_server, (struct sockaddr *)&exp, &fromlen)) < 0) {
			perror("error accept client");
			/* Error: Do what? */
    	}

#ifdef DEBUG
		puts("New client connected!");
#endif    	

    	/* TODO: if IP address exp is blacklisted -> do nothing (pour chaque requete donc pas ici en faite) */

		if(inet_ntop(AF_INET, &((struct sockaddr_in*)&exp)->sin_addr, ip, INET_ADDRSTRLEN) == NULL) { /* thread safe contrairement à inet_ntoa() */
#ifdef DEBUG
			puts("in_addr to string failed");
#endif
		}

		cl = (client *) malloc(sizeof(client)); /* client must free itself in its thread */
		client_init(cl, server, socket_client, ip);
		if(pthread_create(&thread, NULL, client_process_socket, (void*) cl) != 0) {
			perror("Creating client thread");
			/* Error: Do what? */
		}
		
		pthread_detach(thread);

    	if (pthread_mutex_lock(&server->mutex_nbClient) < 0) {
			perror("lock mutex nbClient");
			/* Error: Do what? */
		}

		server->nbClient++;

		while(server->nbClient == server->nbMaxClient) {
#ifdef DEBUG
			puts("Too much clients, server starts sleeping while no client disconnects...");
#endif
			pthread_cond_wait(&server->cond_maxClient, &server->mutex_nbClient);
			/* signaled by client threads */
		}

		if (pthread_mutex_unlock(&server->mutex_nbClient) < 0) {
			perror("unlock mutex nbClient");
			/* Error: Do what? */
		}
	}
}