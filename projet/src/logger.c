#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "logger.h"

int logger_init(logger* log) {

	if ((log->fd = open (LOG_PATH, O_WRONLY|O_APPEND|O_CREAT, 0222)) < 0) { /* O_APPEND => pas besoin d'utiliser lseek pour écrire fin fichier */
#ifdef DEBUG
		perror("error opening log file");
#endif
      	return errno;
    }

    log->mutex = PTHREAD_MUTEX_INITIALIZER;

    return 0;
}

int logger_destroy(logger* log) {
	close(log->fd);
	pthread_mutex_destroy(&log->mutex);
}

void log(logger* log, struct in_addr caddress, time_t time, pid_t spid, pthread_t stid, char* req_line, int retcode, int filesize) {
	/*
		preparer tous les buffers
		const char *inet_ntop(int af, const void *src,
                             char *dst, socklen_t size) thread safe contrairement à inet_ntoa() (vir man pour longueur IP adresse)
		ctime_r(const time_t *time, char *buf) thread safe contrairement à ctime() 
		snprintf + sprintf : http://stackoverflow.com/questions/3919995/determining-sprintf-buffer-size-whats-the-standard
		take mutex
			write to file
		release mutex
		free tous les buffers
	*/
}
