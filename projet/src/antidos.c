#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "antidos.h"

#define BLACKLIST_TIME 10
#define REQUEST_TIME 60

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

void request_summary_init(request_summary *rqsum, time_t date,int size)
{
	rqsum->size = size;
	rqsum->date = date;
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

void list_clean_requests(listNode *client)
{

	client_summary *clsum = (client_summary *) client->data;
	listNode *reqlist = clsum->requests->head;
	request_summary *rqsum;

	while(reqlist != NULL)
	{	
		rqsum = (request_summary *) reqlist->data;
		if(difftime(time(NULL), rqsum->date) > REQUEST_TIME)
		{
			clsum->totalSize -= rqsum->size;
			list_remove(clsum->requests, rqsum);
		}

		else
			break;

		reqlist = reqlist->next;
	}
}

int antidos_add_request(antidos* dos, char ip[INET_ADDRSTRLEN], time_t date, int reqSize)
{
	listNode *client = dos->clients->head;
	client_summary *clsum;

	request_summary *rqsum = (request_summary *) malloc(sizeof(request_summary));
	request_summary_init(rqsum, date, reqSize);
	while(client != NULL)
	{
		clsum = (client_summary *) client->data;
		if(strcmp(clsum->ip,ip) == 0)
		{
			if (pthread_mutex_lock(&clsum->mutex_listReq) < 0)
			{
				perror("lock mutex clientAntidos");
				free(rqsum);
			}

			list_add(clsum->requests, rqsum);
			clsum->totalSize += reqSize;
			list_clean_requests(client);

			if(clsum->totalSize >= dos->seuil)
				clsum->date_blacklist = time(NULL) + BLACKLIST_TIME;
			
			if (pthread_mutex_unlock(&clsum->mutex_listReq) < 0)
			{
				perror("unlock mutex clientAntidos");
			}
			return 0;
		}
		client = client->next;
	}
	return -1;
}

int antidos_is_blacklisted(antidos* dos, char ip[INET_ADDRSTRLEN])
{
	listNode *client = dos->clients->head;
	client_summary *clsum;

	while(client != NULL)
	{
		clsum = (client_summary *) client->data;
		if(strcmp(clsum->ip,ip) == 0)
		{
			if (difftime(time(NULL), clsum->date_blacklist) >= 0)
				return 0;
			printf("Blacklisté: %d\n", clsum->totalSize);
			return 1;
		}
		client = client->next;
	}
	return -1;
}


/*
void bl(antidos dos)
{
	if (antidos_is_blacklisted(&dos, "127.0.0.1"))
	else
		printf("Pas Blacklisté: %s\n", "127.0.0.1");
}

int main()
{
	antidos dos;
	antidos_init(&dos, 2);
	antidos_add_client(&dos, "127.0.0.1");
	antidos_add_request(&dos, "127.0.0.1", time(NULL), 1);
	antidos_add_request(&dos, "127.0.0.1", time(NULL), 1);
	bl(dos);
	sleep(1);
	antidos_add_request(&dos, "127.0.0.1", time(NULL), 1);
	bl(dos);
	sleep(1);
	bl(dos);
	antidos_destroy(&dos);


	return 0;
}

*/