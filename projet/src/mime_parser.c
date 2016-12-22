#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <regex.h>

#include "mime_parser.h"

int mime_parser_init(mime_parser* mp) {

	struct stat st_mime;

	if(stat(MIME_PATH, &st_mime) < 0) {
#ifdef DEBUG
		puts("error stat mimes.types");
#endif
		return errno;
	}

	if ((mp->fd = open (MIME_PATH, O_RDONLY, 0444)) < 0) {
#ifdef DEBUG
		puts("error opening mimes.types");
#endif
      	return errno;
    }

    mp->buffer = (char *) malloc(st_mime.st_size * sizeof(char));

    if(read(mp->fd, mp->buffer, st_mime.st_size) < 0) {
#ifdef DEBUG
		puts("error reading mimes.types");
#endif
		free(mp->buffer);
      	return errno;
    }

    mp->cache = (mime_cache_entry *) malloc(sizeof(mime_cache_entry) * MIME_CACHE_SIZE); 
    mp->cache_index = 0;

    return 0;
}

int mime_parser_destroy(mime_parser* mp) {
	free(mp->buffer);
	free(mp->cache);
	close(mp->fd);
	free(mp);
	return 0;
}

char* parse_file_ext(mime_parser* mp, char* file_ext) {
	char* pattern;
	regex_t reg;
	char err_buf[BUFSIZ];
	regmatch_t match[1];
	int res;
	char* match_offset;

	/*
		(\S+)(.*)(\s)(txt)(\s) => Match 5 groupes, 1er groupe = type mime
	*/
	
	pattern = (char *) malloc((3 + strlen(file_ext) + 3 + 1) * sizeof(char));
	strcpy(pattern, "\\s(");
	strcat(pattern, file_ext);
	strcat(pattern, ")\\s");

	if( (res = regcomp(&reg, pattern, REG_ICASE)) != 0) {
#ifdef DEBUG
		regerror(res, &preg, err_buf, BUFSIZ);
		printf("regcomp: %s\n", err_buf);
#endif
		return res;
	}

	if( (res = regexec(&reg, mp->buffer, 1, match, 0)) != 0) {
#ifdef DEBUG
		regerror(res, &preg, err_buf, BUFSIZ);
		printf("regexec: %s\n", err_buf);
#endif
		return res;
	}

	match_offset = mp->buffer + match[0].rm_so;

	/*

	while((*match_offset) != '\n')
		match_offset--;

	*/
	
	regfree(&reg);
	return 0;

}

