#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

int create_connection(char *host, int port) {
    struct sockaddr_in server_addr;
    struct hostent *server;
    int sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    if ((server = gethostbyname(host)) == NULL) {
        perror("gethostbyname");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        return -1;
    }

    return sock;
}

int main(int argc, char* argv[]) {

    char* host, *filename;
    int port;
    int sock, fdfile;

    if(argc != 3) {
        printf("Bad args\n");
        return EXIT_FAILURE;
    }

    ip = argv[1];
    port = atoi(argv[2]);
    filepath = argv[3];

    if ((fdfile = open (filepath, O_RDONLY)) == -1) {
        perror("open fd");
        return errno;
    }

    if((sock = create_connection(host, port) == -1)) {
        perror("connect");
        return errno;
    }

    filename[strlen(filename)] = '\n';


    close(sock);
    close(fdfile);

}