#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>

#include "mime_parser.h"

int mime_parser_init(mime_parser* mp) {

	struct stat st_mime;

	if(stat(MIME_PATH, &st_mime) < 0) {
#ifdef DEBUG
		perror("error stat mimes.types");
#endif
		return errno;
	}

	if ((mp->fd = open (MIME_PATH, O_RDONLY, 0444)) < 0) {
#ifdef DEBUG
		perror("error opening mimes.types");
#endif
		return errno;
    }

    mp->buffer = (char *) malloc(st_mime.st_size * sizeof(char));

    if(read(mp->fd, mp->buffer, st_mime.st_size) < 0) {
#ifdef DEBUG
		perror("error reading mimes.types");
#endif
		free(mp->buffer);
		return errno;
    }

#ifdef DEBUG
	puts("MIME parser initialized");
#endif

	return 0;
}

int mime_parser_destroy(mime_parser* mp) {
	
	free(mp->buffer);
	close(mp->fd);

#ifdef DEBUG
	puts("MIME parser destroyed");
#endif
	return 0;
}

char* parse_file_ext(mime_parser* mp, char* file_ext) {
	char* pattern;
	regex_t reg;
	char err_buf[ERR_BUFFER_SIZE];
	size_t maxGroups = 6; /* Nb de groupe regex + 1 */
	regmatch_t match[maxGroups];
	int res;
	char* mime_type_str;

	int i = 0;
	int len = 0;
	char result[2048];

	/*
		(\S+)(.*)(\s)(txt)(\s) => Match 5 groupes, 1er groupe = ligne qui match, 2nd = 1er groupe(mime_type)
	*/
	
	len = 14 + strlen(file_ext) + 5 + 1;
	pattern = (char *) malloc(len * sizeof(char));
	strcpy(pattern, "(\\S+)(.*)(\\s)(");
	strcat(pattern, file_ext);
	strcat(pattern, ")(\\s)");
	pattern[len] = '\0';
#ifdef DEBUG
	printf("Pattern: %s\n", pattern);
#endif


	if( (res = regcomp(&reg, pattern, REG_ICASE|REG_EXTENDED|REG_NEWLINE)) != 0) {
#ifdef DEBUG
		regerror(res, &reg, err_buf, ERR_BUFFER_SIZE);
		printf("regcomp error: %s\n", err_buf);
#endif
		return NULL;
	}

	if( (res = regexec(&reg, mp->buffer, maxGroups, match, 0)) != 0) {
#ifdef DEBUG
		regerror(res, &reg, err_buf, ERR_BUFFER_SIZE);
		printf("regexec: %s\n", err_buf);
#endif
		return NULL;
	}

#ifdef DEBUG
	for (i = 0; match[i].rm_so != -1 && i < (int) maxGroups; i++) {
		len = match[i].rm_eo - match[i].rm_so;
		memcpy(result, mp->buffer + match[i].rm_so, len);
		result[len] = '\0';
		printf("num %d: '%s'\n", i, result);
   	}
#endif

   	len = match[1].rm_eo - match[1].rm_so;
   	mime_type_str = (char *) malloc(len + 1);
   	memcpy(mime_type_str, mp->buffer + match[1].rm_so, len);
   	mime_type_str[len] = '\0';
	
	free(pattern);
	regfree(&reg);
	return mime_type_str;

}

int main(int argc, char* argv[]) {
	mime_parser mp;
	char * mime_type_str;

	if(argc != 2) {
		printf("usage: %s <file_ext>\n", argv[0]);
		return 0;
	}

	mime_parser_init(&mp);
	
	mime_type_str = parse_file_ext(&mp, argv[1]);
	printf("Mime_type: '%s'\n", mime_type_str);

	mime_parser_destroy(&mp);

	return 0;
}

