#ifndef HTTP_REQUEST_H_  
#define HTTP_REQUEST_H_

#include <client.h>
#include <semaphore.h>

typedef struct request {
	client* client;
	time_t req_date;
	char *first_line;
	pid_t server_pid;
	pid_t thread_id;
	unsigned short return_code;
	unsigned int data_size; 
	sem_t* sem_can_send;
	sem_t* sem_reply_done;
} request;

void request_init(request *req, client* client, char* req_txt);

void* request_process(void *r);

void request_destroy(request *req);

char* getPathRequested(const char *request);

void send_error404(request *req);

void send_error403(request *req);

void send_200(request *req, int fd, char *fileExt);

void responseDisplayClean(int id, request *req, char *path);

#endif 