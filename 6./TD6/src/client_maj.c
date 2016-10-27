#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

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
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    printf("%s\n", linep);
    return linep;
}

int main(int argc, char* argv[]) {

	int fifofd;
	char* line;

	if(argc != 2) {
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	if(mkfifo(argv[1], 0600) != 0) {
		perror("error mkfifo\n");
		return EXIT_FAILURE;
	}

	if ((fifofd = open(argv[1], O_RDONLY, 0600)) == -1) {
    	perror("error open fifo\n");
     	return EXIT_FAILURE;
    }

    while(1) {
    	line = readline(stdin);
        /*printf("%s\n", line);*/
    	if(strcmp(line, "exit") == 0)
    		break;
    	write(fifofd, line, sizeof(line));
        free(line);
    }

    close(fifofd);
    unlink(argv[1]);

    return EXIT_SUCCESS;
}