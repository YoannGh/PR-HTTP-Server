#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>

#include "http_request.h"
#include "mime_parser.h"
#include "logger.h"

#define BUFFER_SIZE 1024
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KRESET "\x1B[0m"

static char* getPathRequested(const char *request);
static void send_error500(request *req);
static void send_error404(request *req);
static void send_error403(request *req);
static void send_200(request *req, int fd, char *fileExt);
static void responseDisplayClean(int id, request *req, char *path);
static int* processBinaryRequested(request *req, char *path);
static void send_200_binary(request *req, int pipe_fd);
void sig_handler(int sig){}

/* il y a nb_requete+1 semaphores alloués mais (nb_requete+1)*2 références sur ces sem
	la requete N free(prev_req_send) et par conséquent free(reply_done) de la requete N-1
	le thread client est chargé de free(reply_done) de la dernière requête
	en maintenant un pointeur sur le dernier semaphore alloué.
	*/

int request_init(request *req, client* cl, char* fline, sem_t* prev_req, sem_t* req_done)
{
	char method[16];
	char url[strlen(fline)];
	char protocol[16];

	sscanf(fline, "%s %s %s", method, url, protocol);

	/* Only support HTTP version 1.0 and 1.1 */
	if(strcmp(protocol, "HTTP/1.0") && strcmp(protocol, "HTTP/1.1"))
		return -1;
	
	else if (strcmp(method, "GET")) /* Only support GET method */
		return -1;

	req->cl = cl;
	req->req_date = time(NULL);
	req->return_code = 0;
	req->data_size = 0;

	/* on a direct la premiere ligne */
	req->first_line = (char *) malloc((strlen(fline) + 1) * sizeof(char));
	strcpy(req->first_line, fline);

	req->sem_prev_req_sent = prev_req;

	if(sem_init(req_done, 0, 0) < 0) {
		perror("failed to init req done semaphore");	
	}
	req->sem_reply_done = req_done;

#ifdef DEBUG
	printf("Request received: %s\n", req->first_line);
	puts("Request initialized");
#endif

	return 0;
}

void* request_process(void *r)
{
	int fd;
	int *pipe_fd = NULL;
	request *req = (request *) r;
	struct stat sb;
	char *path, *fileExt;
	logger* log;

	log = req->cl->server->log;

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
		pipe_fd = processBinaryRequested(req, path);
	}
	/* last case, simple file */
	else 
	{
		req->data_size = sb.st_size;
		req->return_code = 200;
	}

	sem_wait(req->sem_prev_req_sent);

	if(req->return_code == 403)
		send_error403(req);
	else if(req->return_code == 404)
		send_error404(req);
	else if(req->return_code == 500)
		send_error500(req);
	else if (pipe_fd)
		send_200_binary(req, *pipe_fd);
	else
		send_200(req, fd, fileExt);

#ifdef DEBUG
	responseDisplayClean(getpid(), req, path);
#endif 
	sem_post(req->sem_reply_done);
	/* log processed request */
	log_request(log, req->cl->ip, req->req_date, getpid(), pthread_self(), req->first_line, req->return_code, req->data_size);
	close(fd);
	free(path);
	if (pipe_fd)
		    free(pipe_fd);

	request_destroy(req);

	pthread_exit(NULL);
}

void request_destroy(request *req)
{
	sem_destroy(req->sem_prev_req_sent);
	free(req->sem_prev_req_sent);
	free(req->first_line);
	free(req);

#ifdef DEBUG
	puts("Request destroyed");
#endif
}

static char* getPathRequested(const char *request)
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

static void send_error500(request *req)
{
	char *err500 = 
      "HTTP/1.1 500 Internal Server Error\n"
      "Content-type: text/html\n"
      "Content-Length: 173\n"
      "\n"
      "<html>\n"
      " <body>\n"
      "  <h1>500 Internal Server Error</h1>\n"
      "  <p>The server encountered an unexpected condition which prevented it from fulfilling the request.</p>\n"
      " </body>\n"
      "</html>\n";

	req->data_size = strlen(err500);
	if(write(req->cl->socket, err500, strlen(err500)) == -1)
   	{
		perror("Write 500");
    	return;
   	}
}

static void send_error404(request *req)
{
	char *err404 = 
      "HTTP/1.1 404 Not Found\n"
      "Content-type: text/html\n"
      "Content-Length: 114\n"
      "\n"
      "<html>\n"
      " <body>\n"
      "  <h1>404 Not Found</h1>\n"
      "  <p>The requested URL was not found on this server.</p>\n"
      " </body>\n"
      "</html>\n";

	req->data_size = strlen(err404);
	if(write(req->cl->socket, err404, strlen(err404)) == -1)
   	{
		perror("Write 404");
    	return;
   	}
}

static void send_error403(request *req)
{

	char *err403 = 
      "HTTP/1.1 403 Forbidden\n"
      "Content-type: text/html\n"
      "Content-Length: 110\n"
      "\n"
      "<html>\n"
      " <body>\n"
      "  <h1>403 Forbidden</h1>\n"
      "  <p>You don't have access to the requested URL.</p>\n"
      " </body>\n"
      "</html>\n";

	req->data_size = strlen(err403);
	if(write(req->cl->socket, err403, strlen(err403)) == -1)
   	{
		perror("Write 403");
    	return;
   	}
}

static void send_200(request *req, int fd, char *fileExt)
{
	int n;
	char buffer[BUFFER_SIZE];
	char *mime_txt;
	char *header;
	int header_length;

	if(parse_file_ext(fileExt, &mime_txt) < 0) {
#ifdef DEBUG
		puts("Error parsing file extension");
#endif		
	}

	header_length = snprintf(NULL, 0, "HTTP/1.1 200 OK\nContent-type: %s\nContent-Length: %d\n\n", mime_txt, req->data_size);
	header = (char *) malloc((header_length + 1) * sizeof(char));
	sprintf(header, "HTTP/1.1 200 OK\nContent-type: %s\nContent-Length: %d\n\n", mime_txt, req->data_size);
	header[header_length] = '\0';

	free(mime_txt);

    printf("Header reply: '%s'\n", header);

    if(write(req->cl->socket, header, strlen(header)) == -1)
    {
    	perror("Write 200 header");   	
		free(header);
    	return;
    }

	while((n = read(fd, buffer, BUFFER_SIZE)) != 0)
	{
		if (n == -1)
		{
			perror("readFile");
			free(header);
			return;
		}

    	if(write(req->cl->socket, buffer, n) == -1)
   	    {
   		 	perror("Write 200");
			free(header);
    		return;
	    }
	}

	free(header);
}

static void responseDisplayClean(int id, request *req, char *path)
{
	if (req->return_code == 200)
		printf("ID %d processed request \"%s\" | Size: %d, answer: %d [" KGRN "OK" KRESET"]\n", id, path, req->data_size, req->return_code);
	else
		printf("ID %d processed request \"%s\" | Size: %d, answer: %d [" KRED "KO" KRESET"]\n", id, path, req->data_size, req->return_code);
}


static int* processBinaryRequested(request *req, char *path)
{
	int filedes[2], pid, statval, rc;
	int *ret;
	struct timeval timeout = {10,0};
	/* Child process will exec the binary and send the output through the pipe */
	if (pipe(filedes) == -1) {
		perror("pipe_Binary");
		req->return_code = 500;
		return NULL;
	}

	if ((pid = fork()) == -1) {
		perror("fork_Binary");
		req->return_code = 500;
		return NULL;
	}

	//Child
	if (pid == 0)
	{
		while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
		close(filedes[1]);
		close(filedes[0]);
		execl(path, path, (char*)0);
		perror("execl");
		req->return_code = 500;
		return NULL;
	}
	//Father
	close(filedes[1]);

	signal(SIGCHLD, sig_handler);

	//Select will get interrupted by a signal or timeout
	rc = select(0, NULL,NULL,NULL, &timeout);

	//Timed_out
	if(rc == 0)
		req->return_code = 500;
	//Normal Execution 
	else
	{
		//wait(&statval);
		waitpid(pid, &statval, WNOHANG);
		if(WIFEXITED(statval))
		{
			if(WEXITSTATUS(statval) != 0)
				req->return_code = 500;
			else
				req->return_code = 200;
		}
		else
			req->return_code = 500;
	}
	ret = malloc(sizeof(int));
	*ret = filedes[0];
	return ret;
}

static void send_200_binary(request *req, int pipe_fd)
{
	int n, header_length;
	char buffer[BUFFER_SIZE];
	char *header;

	/* TODO que faire si output > buffer ? 
		- Soit on fixe un output max
		- Soit on boucle sur le pipe (pour le content-length), mais faut realloc
	*/

	n = read(pipe_fd, buffer, BUFFER_SIZE);
	req->data_size = n; 

	if (n == -1)
	{
		perror("read_pipe");
		return;
	}

	header_length = snprintf(NULL, 0, "HTTP/1.1 200 OK\nContent-type: text/plain\nContent-Length: %d\n\n", req->data_size);
	header = (char *) malloc((header_length + 1) * sizeof(char));
	sprintf(header, "HTTP/1.1 200 OK\nContent-type: text/plain\nContent-Length: %d\n\n", req->data_size);
	header[header_length] = '\0';

	if(write(req->cl->socket, header, strlen(header)) == -1)
	{
		perror("Write Header 200 Binary");
		return;
	}

	if(write(req->cl->socket, buffer, n) == -1)
	{
		perror("Write 200 Binary");
		return;
    }

    free(header);
} 