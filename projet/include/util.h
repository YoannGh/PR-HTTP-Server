#ifndef UTIL_H_
#define UTIL_H_

char* readable_filesize(double size, char *buf);

int readline(int fd, char ** line);

int set_nonblock(int fd);

#endif