#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <linux/limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>


#define BUFFER_SIZE 1024
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KRESET "\x1B[0m"


typedef struct request {
	int socket;
	time_t req_date;
	char *first_line;
	char client_ip_adress[15];
	pid_t server_pid;
	pid_t thread_id;
	unsigned short return_code;
	unsigned int data_size; 
} request;

void init_request(request *req, int socket)
{
	char buffer[BUFFER_SIZE];
    char *ptr, *save;
    int n;

	memset(req->client_ip_adress, 0, 15);
	req->req_date = time(NULL);
	req->socket = socket;
	req->return_code = 0;
	req->data_size = 0;

	if((n = recv(socket, buffer, BUFFER_SIZE, 0)) < 0) {
		perror("readSocket");
	}

	ptr = buffer;
	while(*ptr != '\n')
		ptr++;

	//On sauvegarde la première ligne de la requete
	req->first_line = malloc((ptr-buffer) +1);
	memcpy(req->first_line, buffer, ptr-buffer);

	//On recupere l'adresse ip sur la seconde ligne
	ptr+=6;
	save = ptr;

	while(*ptr != '\n')
		ptr++;

	memcpy(req->client_ip_adress, save, ptr-save);
}

void free_request(request *req)
{
	free(req->first_line);
}

void display_request(request *req)
{
	puts("Request: ");
	printf("First line: %s\n", req->first_line);
	printf("ip adress: %s\n", req->client_ip_adress);
	printf("Code: %d\n", req->return_code);
	printf("Data sent : %do\n", req->data_size);
	puts("");
}

void cleanResponseDisplay(int id, request *req, char *path)
{
	if (req->return_code == 200)
		printf("ID %d processed request \"%s\" | Size: %d, answer: %d [" KGRN "OK" KRESET"]\n", id, path, req->data_size, req->return_code);
	else
		printf("ID %d processed request \"%s\" | Size: %d, answer: %d [" KRED "KO" KRESET"]\n", id, path, req->data_size, req->return_code);
}



int init_server(int port, int maxclient) {
	struct sockaddr_in sin;
	int sock;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	listen(sock, maxclient);

	return sock;
}

char *get_filename_ext(const char *filename) {
    char *dot = strrchr(filename, '.');

    if(!dot || dot == filename) 
    	return "";
    return dot + 1;
}



void send_error404(request *req)
{
	char *err404 = 
      "HTTP/1.1 404 Not Found\n"
      "Content-type: text/html\n"
      "\n"
      "<html>\n"
      " <body>\n"
      "  <h1>Not Found</h1>\n"
      "  <p>The requested URL was not found on this server.</p>\n"
      " </body>\n"
      "</html>\n";

	req->return_code = 404;
	req->data_size = strlen(err404);
	write(req->socket, err404, strlen(err404));
}

void send_error403(request *req)
{
	char *err403 = 
      "HTTP/1.1 403 Forbidden\n"
      "Content-type: text/html\n"
      "\n"
      "<html>\n"
      " <body>\n"
      "  <h1>Forbidden</h1>\n"
      "  <p>You don't have access to the requested URL.</p>\n"
      " </body>\n"
      "</html>\n";

	req->return_code = 403;
	req->data_size = strlen(err403);
	write(req->socket, err403, strlen(err403));
}


char* getPathRequested(const char *request)
{
	char* path;
	char* travel;
	path = malloc(sizeof(char));
	*path = '\0';
	travel = path;
	request+=5;

	while(*request != ' ')
	{
		*travel++ = *request;
		request++;
	}
	*travel = '\0';
	path = realloc(path, (travel-path)+1);
	return path;
}

void reply(request *req)
{
	int fd, n;
	char *path, *header;
	char buffer[BUFFER_SIZE];

	path = getPathRequested(req->first_line);

	if ((fd = open(path, O_RDONLY, 0600)) == -1)
	{
		if(errno == EACCES)
			send_error403(req);
		else
			send_error404(req);
		cleanResponseDisplay(getpid(), req, path);
		return;
	}

	req->return_code = 200;
	header = 
	  "HTTP/1.1 200 OK\n"
      "Content-type: text/html\n"
      "\n";

    write(req->socket, header, strlen(header));
	req->data_size+= strlen(header);

	while((n = read(fd, buffer, BUFFER_SIZE)) != 0)
	{
		if (n == -1)
		{
			goto end;
			perror("readFile");
		}
		req->data_size+= n;
    	write(req->socket, buffer, n);
	}

	cleanResponseDisplay(getpid(), req, path);
	end:
	close(fd);
	free(path);
}


void processSocket(int sock)
{
	request *req;

	req = malloc(sizeof(request));

	init_request(req, sock);
	reply(req);

	free_request(req);
	free(req);
}

int main(int argc, char* argv[]) {

	struct sockaddr_in exp;
	int sockclient, i;
	socklen_t fromlen;
	int* sockTab;
	fd_set mselect;

	if(argc < 2) {
        printf("Bad args\n");
        return EXIT_FAILURE;
    }

    FD_ZERO(&mselect);

    sockTab = malloc(sizeof(int)*(argc-1));


	for(i=1; i<argc; ++i)
    {
    	sockTab[i-1] = init_server(atoi(argv[i]), 120);
    	FD_SET(sockTab[i-1], &mselect);
    }

    //Entrée standard pour fermer le serv
    FD_SET(0,&mselect);

    while(1) {

    	puts("awaiting client");

		if(select(sockTab[i-2]+1, &mselect, NULL, NULL, NULL) == -1)
		{
			perror("select");
			return errno;
		}
    	if(FD_ISSET(0, &mselect))
			break;

    	puts("client connected");
    	for(i=0; i<argc-1; ++i)
    	{
    		if(FD_ISSET(sockTab[i], &mselect))
    		{
    			if((sockclient = accept(sockTab[i], (struct sockaddr *)&exp, &fromlen)) < 0) {
					perror("accept");
					return errno;
    			}


    			processSocket(sockclient);
    			shutdown(sockclient, 2);
    			printf("IP address is: %s\n", inet_ntoa(exp.sin_addr));
			}
    	}
    	
    	FD_ZERO(&mselect);
        for(i=1; i<argc; ++i)
	    {
	    	FD_SET(sockTab[i-1], &mselect);
	    }
    	FD_SET(0,&mselect);
    }

    puts("Closing http server...");
    for(i=0; i<argc-1; ++i)
    {
    	shutdown(sockTab[i], 2);
    	close(sockTab[i]);
    }


    return EXIT_SUCCESS;
}