#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

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

int main(int argc, char* argv[]) 
{
	char* ip;
	int port;
	int socketfd;
	struct addrinfo *info;
	struct sockaddr_in dest;

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

	if (getaddrinfo(argv[1], NULL, NULL, &addr) != 0) {
		perror("getaddrinfo"); exit(EXIT_FAILURE);
	}

	memset((char *)&dest,0, sizeof(dest));
	memcpy ((void*)&dest.sin_addr,
	(void*)&((struct sockaddr_in*)result->ai_addr)->sin_addr, sizeof(dest));
	dest.sin_family = AF_INET;




}