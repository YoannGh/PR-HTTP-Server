#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include <semaphore.h>

#include "client.h"

typedef struct request {
	client* cl;
	time_t req_date;
	char *first_line;
	unsigned short return_code;
	unsigned int data_size;
	sem_t* sem_prev_req_sent;
	sem_t* sem_reply_done;
} request;

int request_init(request *req, client* cl, char* fline, sem_t* prev_req, sem_t* req_done);

void* request_process(void *r);

void request_destroy(request *req);

#endif 