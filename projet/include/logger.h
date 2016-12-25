#include <pthread.h>

#define LOG_PATH "/tmp/http3506454.log"

typedef struct logger {
	int fd;
	pthread_mutex_t mutex;
} logger;

int logger_init(logger* log);

int logger_destroy(logger* log);

void log(logger* log, struct in_addr caddress, time_t time, pid_t spid, pthread_t stid, char* req_firstline, int retcode, int filesize);