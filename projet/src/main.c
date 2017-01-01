#include <stdlib.h>
#include <stdio.h>

#include "http_server.h"

int main(int argc, char* argv[]) {

	int numPort;
	int nbMaxClient;
	int antiDOS;
	http_server* server;

	if(argc < 4) {
        printf("usage: %s <num_port> <nb_max_client> <antiDOS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numPort = atoi(argv[1]);
    nbMaxClient = atoi(argv[2]);
	antiDOS = atoi(argv[3]);

	server = (http_server*) malloc(sizeof(http_server));

	http_server_init(server, numPort, nbMaxClient, antiDOS);

	http_server_run_loop(server);

	http_server_destroy(server);

	free(server);

	exit(EXIT_SUCCESS);
}