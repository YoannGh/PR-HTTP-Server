#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <pthread.h>

#include "logger.h"
#include "mime_parser.h"
#include "antidos.h"

typedef struct http_server {
	logger* log;
	int numPort;
	int nbMaxClient;
	antidos *antiDOS;
	int nbClient;
	pthread_mutex_t mutex_nbClient;
	pthread_cond_t cond_maxClient;
} http_server;

void http_server_init(http_server* server, int numPort, int nbMaxClient, antidos *antiDOS);

void http_server_run_loop(http_server* server);

void http_server_destroy(http_server* server);

#endif