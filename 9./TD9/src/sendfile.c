#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <linux/limits.h>
#include <libgen.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

int create_connection(char *host, int port) {
    struct sockaddr_in server_addr;
    struct addrinfo hints, *res;
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    if(getaddrinfo(host, NULL, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    /*if ((server = gethostbyname(host)) == NULL) {
        perror("gethostbyname");
        return -1;
    }*/

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    /*memcpy(&server_addr.sin_addr.s_addr, server->h_addr_list, server->h_length);*/
    server_addr.sin_addr.s_addr = ((struct sockaddr_in *) (res->ai_addr))->sin_addr.s_addr;
    server_addr.sin_port = htons(port);

    printf("server ip: %s\n", inet_ntoa(server_addr.sin_addr));

    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        return -1;
    }

    puts("connect done");

    return sock;
}

int main(int argc, char* argv[]) {

    char* host;
    char* filepath;
    char* filename, *fname;
    int port;
    int sock, fdfile;
    int nread;
    char buffer[BUFFER_SIZE];

    if(argc != 4) {
        printf("Bad args\n");
        return EXIT_FAILURE;
    }

    host = argv[1];
    port = atoi(argv[2]);
    filepath = argv[3];

    if ((fdfile = open (filepath, O_RDONLY)) < 0) {
        perror("open fd");
        return errno;
    }

    puts("open done");

    fname = malloc(NAME_MAX);
    memset(fname, 0, NAME_MAX);
    filename = basename(filepath);
    memcpy(fname, filename, strlen(filename));

    if((sock = create_connection(host, port)) < 0) {
        perror("connect");
        return errno;
    }

    printf("sock: %d\n", sock);

    puts("connection done");

    printf("sending file: %s\n", fname);

    if(send(sock, fname, NAME_MAX, 0) < 0) {
        perror("send filename");
        printf("errno: %d\n", errno);
        return EXIT_FAILURE;
    }

    puts("Avant sleep");
    sleep(10);
    puts("Apres sleep");

    puts("sent filename");

    while((nread = read(fdfile, buffer, BUFFER_SIZE)) > 0) {
        if(send(sock, buffer, nread, 0) < 0) {
            perror("send data");
            return EXIT_FAILURE;
        }
    }

    puts("end while");

    close(sock);
    close(fdfile);
    free(fname);

    return EXIT_SUCCESS;

}