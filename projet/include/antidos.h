#ifndef __ANTIDOS_H
#define __ANTIDOS_H

#include <netinet/in.h> /* for INET_ADDRSTRLEN macro */
#include <pthread.h>

#include "list.h"

typedef struct request_summary {
	int size;
	time_t date;
} request_summary;

typedef struct client_summary {
	char ip[INET_ADDRSTRLEN];
	list* requests; /* list of request_summary */
	time_t date_blacklist;
	pthread_mutex_t mutex_listReq;
	int totalSize;
} client_summary;

typedef struct antidos {
	list* clients; /* list of client_summary */
	int seuil;
} antidos;

void antidos_init(antidos* dos, int seuil);

void antidos_destroy(antidos* dos);

int antidos_add_client(antidos* dos, char ip[INET_ADDRSTRLEN]);

int antidos_add_request(antidos* dos, char ip[INET_ADDRSTRLEN], time_t date, int reqSize);

int antidos_is_blacklisted(antidos* dos, char ip[INET_ADDRSTRLEN]);

#endif
