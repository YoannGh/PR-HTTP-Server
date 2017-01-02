#ifndef MIME_PARSER_H_
#define MIME_PARSER_H_

#define MIME_PATH "/etc/mime.types"
#define ERR_BUFFER_SIZE 1024
#define DEFAULT_MIME_TYPE "text/plain"

int parse_file_ext(char* file_ext, char** type);

#endif