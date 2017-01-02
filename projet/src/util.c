#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

char* readable_filesize(double size, char *buf) {
    int i = 0;
    const char* units[] = {"o", "Ko", "Mo", "Go", "To", "Po", "Eo", "Zo", "Yo"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }
    sprintf(buf, "%.*f%s", i, size, units[i]);
    return buf;
}

int readline(int fd, char ** line) { 
    int buf_size = 128;
    int bytesloaded = 0; 
    int ret;
    char buf; 
    char * buffer = malloc(buf_size); 
    char * newbuf; 

    if (NULL == buffer)
        return -1;

    do
    {
        ret = read(fd, &buf, 1);
        if(buf == '\n' && bytesloaded == 0) {
            buffer[bytesloaded] = '\0';
            *line = buffer;
            return bytesloaded;
        }
        if(ret == 0)
            break;
        else if (ret < 1)
        {
            if(errno == EINTR)
                continue;
            else {
                free(buffer);
                return -1;
            }
        }

        if (buf == '\n')
            break;

        buffer[bytesloaded] = buf; 
        bytesloaded++;

        if (bytesloaded >= buf_size) 
        { 
            buf_size *= 2; 
            newbuf = realloc(buffer, buf_size); 

            if (NULL == newbuf) 
            { 
                free(buffer);
                return -1;
            } 

            buffer = newbuf; 
        }
    } while(1);

    if(!bytesloaded)
        return -1;        

    if ((bytesloaded) && (buffer[bytesloaded-1] == '\r'))
        bytesloaded--;

    buffer[bytesloaded] = '\0';

    *line = buffer;
    return bytesloaded;
}

int set_nonblock(int fd) {
    int flags;
    flags = fcntl(fd, F_GETFL, 0);
    if(flags < 0)
        return -1;
    if(fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        return -1;
    return 0;
}