#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include "env_message.h"


void printMess(struct env_message* mess)
{
	printf("Type: %c\n", mess->type);
	if(mess->type != 'Q')
		printf("Identificateur: %s\n", mess->identificateur);
	if(mess->type == 'S')
		printf("Valeur: %s\n", mess->valeur);
}


int main(int argc, char* argv[]) 
{
	int port;
	int sock;
	struct sockaddr_in sin;
	struct sockaddr_in exp;
	struct env_message mess;
	socklen_t fromlen;

	if(argc != 2) {
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	port = atoi(argv[1]);	

	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	memset((char *)&sin,0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;



	if(bind(sock, (struct sockaddr*) &sin, sizeof(sin)) == -1) {
		perror("bind");
		return EXIT_FAILURE;
	}

	while(1)
	{
		recvfrom(sock, (void *) &mess, sizeof(struct env_message), 0, (struct sockaddr *)&exp, &fromlen);
		puts("Serveur recoit:\n");
		printMess(&mess);
		switch(mess.type)
		{
			case 'S':
				setenv(mess.identificateur, mess.valeur, 1);
				break;
			case 'G':
				strcpy(mess.valeur, getenv(mess.identificateur));
				sendto(sock, (const void*) &mess, sizeof(struct env_message), 0, (const struct sockaddr *)&exp, sizeof(exp));
				break;
			case 'Q':
				break;
			default:
				printf("C LA MERDE\n");
		}
	}

	close(sock);

	return EXIT_SUCCESS;

}