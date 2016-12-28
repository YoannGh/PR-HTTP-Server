#include <pthread.h>

typedef struct http_server {
	logger* log;
	int numPort;
	int nbMaxClient;
	int antiDOS;
	int nbClient;
	pthread_mutex_t mutex_nbClient;
	pthread_cond_t cond_maxClient;
} http_server;

http_server_init(http_server* server, int numPort, int nbMaxClient, int antiDOS);

http_server_run_loop(http_server* server);

http_server_destroy(http_server* server);