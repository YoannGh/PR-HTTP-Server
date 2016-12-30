#include <http_request.h>
#include <mime_parser.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFFER_SIZE 1024
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KRESET "\x1B[0m"

void request_init(request *req, client* client, char* req_txt)
{
	char *ptr;

	req->client = client;
	req->req_date = time(NULL);
	req->return_code = 0;
	req->data_size = 0;

	ptr = req_txt;
	while(*ptr != '\n')
		ptr++;

	//On sauvegarde la première ligne de la requete
	req->first_line = malloc((ptr-req_txt) +1);
	memcpy(req->first_line, req_txt, ptr-req_txt);
	req->first_line[ptr-req_txt] = '\0'; 
}

void* request_process(void *r)
{
	int fd;
	request *req = r;
	struct stat sb;
	char *path, *fileExt;

	path = getPathRequested(req->first_line);
	fileExt = strrchr(path, '.');

	if ((fd = open(path, O_RDONLY, 0600)) == -1)
	{
		if(errno == EACCES)
			req->return_code = 403;
		else
			req->return_code = 404;
	}
	/* If file is a binary */
	else if (fstat(fd, &sb) == 0 && sb.st_mode & S_IXUSR) 
	{
		/* TODO fork() */
		puts("Fichier executable");
	}
	else
		req->return_code = 200;

	//sem_wait(req->sem_can_send);

	if(req->return_code == 403)
		send_error403(req);
	else if(req->return_code == 404)
		send_error404(req);
	else
		send_200(req, fd, fileExt);

#ifdef DEBUG
	responseDisplayClean(getpid(), req, path);
#endif 
	//sem_post(req->sem_reply_done);
	close(fd);
	free(path);
	request_destroy(req);

	return NULL;
}

void request_destroy(request *req)
{
	free(req->first_line);
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

	req->data_size = strlen(err404);
	if(write(req->client->socket, err404, strlen(err404)) == -1)
   	{
		perror("Write 404");
    	return;
   	}
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

	req->data_size = strlen(err403);
	if(write(req->client->socket, err403, strlen(err403)) == -1)
   	{
		perror("Write 403");
    	return;
   	}
}

void send_200(request *req, int fd, char *fileExt)
{
	int n;
	char buffer[BUFFER_SIZE];

	mime_parser mp;
	char *mime_txt, *res;
	char *header = 
	  "HTTP/1.1 200 OK\n"
      "Content-type: ";

	mime_parser_init(&mp);
	mime_txt = parse_file_ext(&mp, fileExt);
	mime_parser_destroy(&mp);

	//+3 pour \n,\n et \0
    res = calloc(1, strlen(header) + strlen(mime_txt) + 3);

    printf("RES: %s\n", res);
    printf("HEADER: %s\n", header);
    strcat(res, header);
    strcat(res, mime_txt);
    strcat(res, "\n\n");


    if(write(req->client->socket, res, strlen(res)) == -1)
    {
    	perror("Write 200 header");
    	return;
    }

	req->data_size+= strlen(header);

	while((n = read(fd, buffer, BUFFER_SIZE)) != 0)
	{
		if (n == -1)
		{
			perror("readFile");
			return;
		}
		req->data_size+= n;
    	if(write(req->client->socket, buffer, n) == -1)
   	    {
   		 	perror("Write 200");
    		return;
	    }
	}

	free(mime_txt);
	free(res);
}

void responseDisplayClean(int id, request *req, char *path)
{
	if (req->return_code == 200)
		printf("ID %d processed request \"%s\" | Size: %d, answer: %d [" KGRN "OK" KRESET"]\n", id, path, req->data_size, req->return_code);
	else
		printf("ID %d processed request \"%s\" | Size: %d, answer: %d [" KRED "KO" KRESET"]\n", id, path, req->data_size, req->return_code);
}