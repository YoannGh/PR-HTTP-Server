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


char * readline(FILE *stream) {
    char * line = malloc(100), * linep = line;
    char * linen;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stream);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
            printf("%s\n", line);
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}

void initMessage(struct env_message* mess, char* str)
{
	char* buffer;
	mess->type = str[0];
	if(str[0] != 'Q')
	{	
		buffer = &str[2];
		while ( ((*buffer) != ' ') && ((*buffer) != '\n') && ((*buffer) != '\0') ) 
			buffer++;

		*buffer = '\0';
		strcpy(mess->identificateur, &str[2]);
		
		if(str[0] != 'G')
		{
			buffer++;
			*(buffer + strlen(buffer)-1) = '\0';
			strcpy(mess->valeur, buffer);
		}
	}
}

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
	char* ip, *ptr;
	int port;
	int socketfd;
	struct addrinfo *info;
	struct sockaddr_in dest;
	struct env_message mess;




	if(argc != 3) {
		printf("Bad args\n");
		return EXIT_FAILURE;
	}


	ip = argv[1];
	port = atoi(argv[2]);	

	if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return EXIT_FAILURE;
	}

	if (getaddrinfo(ip, NULL, NULL, &info) != 0) {
		perror("getaddrinfo"); exit(EXIT_FAILURE);
	}

	memset((char *)&dest,0, sizeof(dest));
	memcpy ((void*)&dest.sin_addr, (void*)&((struct sockaddr_in*)info->ai_addr)->sin_addr, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);


	do
	{
		ptr = readline(stdin);
		if(strlen(ptr) == 1)
			break;
		initMessage(&mess, ptr);
		sendto(socketfd, (const void*) &mess, sizeof(struct env_message), 0, (const struct sockaddr *)&dest, sizeof(dest));

		if(mess.type == 'G')
		{
			recvfrom(socketfd, (void *) &mess, sizeof(struct env_message), 0, NULL, 0);
			printf("Client, reponse: %s\n", mess.valeur);

		}
		free(ptr);
	}while(ptr);
	free(ptr);

	close(socketfd);

	return EXIT_SUCCESS;

}