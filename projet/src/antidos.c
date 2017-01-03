#include <stdlib.h>
#include <string.h>

#include "antidos.h"

int request_summary_equals(void* elem1, void* elem2) { /* void* for genericity */
	if(elem1 == elem2)
		return 1;
	else
		return 0;
}

int client_summary_equals(void* elem1, void* elem2) { /* void* for genericity */
	client_summary* clsum1 = (client_summary *) elem1;
	client_summary* clsum2 = (client_summary *) elem2;

	if(strcmp(clsum1->ip, clsum2->ip) == 0)
		return 1;
	else
		return 0;
}

void client_summary_init(client_summary* clsum, char ip[INET_ADDRSTRLEN]) {

	strncpy(clsum->ip, ip, INET_ADDRSTRLEN);
	clsum->requests = (list *) malloc(sizeof(list));
	list_init(clsum->requests, sizeof(request_summary), request_summary_equals, NULL);
	clsum->date_blacklist = 0;
	pthread_mutex_init(&clsum->mutex_listReq, NULL);
}

void client_summary_destroy(void* elem) { /* void* for genericity */
	client_summary* clsum = (client_summary *) elem;

	pthread_mutex_destroy(&clsum->mutex_listReq);
	list_destroy(clsum->requests);
	free(clsum->requests);
}


void antidos_init(antidos* dos, int seuil) {

	dos->seuil = seuil;
	dos->clients = (list *) malloc(sizeof(list));
	list_init(dos->clients, sizeof(client_summary), client_summary_equals, client_summary_destroy);
}

void antidos_destroy(antidos* dos) {

	list_destroy(dos->clients);
	free(dos->clients);
}

int antidos_add_client(antidos* dos, char ip[INET_ADDRSTRLEN]) {
	client_summary* clsum;

	clsum = (client_summary *) malloc(sizeof(client_summary));
	client_summary_init(clsum, ip);

	if(list_contains(dos->clients, clsum) == 1) {
		free(clsum);
		return 1;
	} else {
		list_add(dos->clients, clsum);
		return 0;
	}
}