#include <stdlib.h>
#include <stdio.h>

#include "http_server.h"
#include "antidos.h"

int main(int argc, char* argv[]) {

	int numPort;
	int nbMaxClient;
	antidos *dos;
	http_server* server;

	if(argc < 4) {
        printf("usage: %s <num_port> <nb_max_client> <antiDOS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numPort = atoi(argv[1]);
    nbMaxClient = atoi(argv[2]);

	dos = (antidos*) malloc(sizeof(antidos));

	server = (http_server*) malloc(sizeof(http_server));

	antidos_init(dos, atoi(argv[3]));

	http_server_init(server, numPort, nbMaxClient, dos);

	http_server_run_loop(server);

	http_server_destroy(server);

	free(server);

	free(dos);

	exit(EXIT_SUCCESS);
}