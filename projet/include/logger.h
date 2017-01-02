#ifndef LOGGER_H_
#define LOGGER_H_

#include <pthread.h>
#include <time.h>
#include <netinet/in.h> /* for INET_ADDRSTRLEN macro */
#include <sys/types.h>

#define LOG_PATH "/tmp/http3506454.log"

typedef struct logger {
	int fd;
	pthread_mutex_t mutex;
} logger;

int logger_init(logger* log);

int logger_destroy(logger* log);

void log_request(logger* log, char ip[INET_ADDRSTRLEN], time_t time, pid_t spid, pthread_t stid, char* request_fline, int retcode, double filesize);

#endif