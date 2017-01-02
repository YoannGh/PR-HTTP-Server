#ifndef MIME_PARSER_H_
#define MIME_PARSER_H_

#define MIME_PATH "/etc/mime.types"
#define ERR_BUFFER_SIZE 1024
#define DEFAULT_MIME_TYPE "text/plain"

typedef struct mime_parser {
	int fd;
	char* buffer;
} mime_parser;

int mime_parser_init(mime_parser* mp);

int parse_file_ext(mime_parser* mp, char* file_ext, char** type);

int mime_parser_destroy(mime_parser* mp);

#endif