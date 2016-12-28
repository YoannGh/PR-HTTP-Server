#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "util.h"
	
int logger_init(logger* log) {

	if ((log->fd = open (LOG_PATH, O_WRONLY|O_APPEND|O_CREAT, 0222)) < 0) { /* O_APPEND => pas besoin d'utiliser lseek pour écrire fin fichier */
#ifdef DEBUG
		perror("error opening log file");
#endif
      	return errno;
    }

    log->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    return 0;
}

int logger_destroy(logger* log) {
	close(log->fd);
	pthread_mutex_destroy(&log->mutex);
	return 0;
}

void log_request(logger* log, struct in_addr caddress, time_t time, pid_t spid, pthread_t stid, char* request, int retcode, double filesize) {

	char ip[INET_ADDRSTRLEN];
	char date[32];
	char* request_fline;
	char buf_fsize[10];
	int line_size = 0;
	char *log_line;

	if(inet_ntop(AF_INET, &caddress, ip, INET_ADDRSTRLEN) == NULL) { /* thread safe contrairement à inet_ntoa() */
#ifdef DEBUG
		puts("in_addr to string failed");
#endif
	}

	if(ctime_r(&time, date) == NULL) { /* thread safe contrairement à ctime() */
#ifdef DEBUG
		puts("time to string failed");
#endif
	}
	date[strlen(date) - 1] = '\0';

	request_fline = strrchr(request, '\n');
	line_size = request_fline - request;
	request_fline = (char *) malloc((line_size + 1) * sizeof(char));
	memcpy(request_fline, request, line_size);
	request_fline[line_size] = '\0';

	readable_filesize(filesize, buf_fsize);

	/* snprintf + sprintf : http://stackoverflow.com/questions/3919995/determining-sprintf-buffer-size-whats-the-standard */
	line_size = snprintf(NULL, 0, "%s %s %d %lu %s %d %s\n", ip, date, (int) spid, (unsigned long) stid, request_fline, retcode, buf_fsize);
	log_line = (char *) malloc((line_size + 1) * sizeof(char));
	sprintf(log_line, "%s %s %d %lu %s %d %s\n", ip, date, (int) spid, (unsigned long) stid, request_fline, retcode, buf_fsize);
	log_line[line_size] = '\0';

	if (pthread_mutex_lock (&log->mutex) < 0) {
		perror("lock mutex logger");
		goto clean;
	}

	if(write(log->fd, log_line, line_size) < line_size) {
#ifdef DEBUG
		printf("error write to log file %s\n", LOG_PATH);
#endif		
	}
	
	if (pthread_mutex_unlock(&log->mutex) < 0) {
		perror("unlock mutex logger");
		goto clean;
	}

clean:
	free(request_fline);
	free(log_line);

}

int main() {
	logger log;
	struct sockaddr_in sin;

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(8888);
	sin.sin_family = AF_INET;

	logger_init(&log);

	log_request(&log, sin.sin_addr, time(NULL), getpid(), pthread_self(), "GET /chemin HTTP/1.1\nHost: 127.0.0.1", 200, 2048);

	logger_destroy(&log);

	printf("Written to logfile %s\n", LOG_PATH);

	return 0;
}