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

int parse_file_ext(mime_parser* mp, char* file_ext, char** type) {
	char* pattern;
	regex_t* reg;
	char err_buf[ERR_BUFFER_SIZE];
	size_t maxGroups = 6; /* Nb de groupe regex + 1 */
	regmatch_t match[maxGroups];
	int res;
	char* mime_type_str;
	int len = 0;

	if(file_ext == NULL) {
		len = strlen(DEFAULT_MIME_TYPE);
		mime_type_str = (char *) malloc(len + 1);
		strcpy(mime_type_str, DEFAULT_MIME_TYPE);
		mime_type_str[len] = '\0';
		*type = mime_type_str;
		return 0;
	}

	if(file_ext[0] == '.') 
		file_ext++;

	reg = (regex_t *) malloc(sizeof(regex_t) + 1000); /* overallocation otherwise memory corruption when calling regfree(reg), dont know why */

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


	if( (res = regcomp(reg, pattern, REG_ICASE|REG_EXTENDED|REG_NEWLINE)) != 0) {
#ifdef DEBUG
		regerror(res, reg, err_buf, ERR_BUFFER_SIZE);
		printf("regcomp error: %s\n", err_buf);
#endif
		return -1;
	}

	if( (res = regexec(reg, mp->buffer, maxGroups, match, 0)) != 0) {
#ifdef DEBUG
		regerror(res, reg, err_buf, ERR_BUFFER_SIZE);
		printf("regexec: %s\n", err_buf);
#endif
		return -1;
	}

   	len = match[1].rm_eo - match[1].rm_so;
   	mime_type_str = (char *) malloc(len + 1);
   	memcpy(mime_type_str, mp->buffer + match[1].rm_so, len);
   	mime_type_str[len] = '\0';
   	*type = mime_type_str;
	
	free(pattern);
	regfree(reg);
	free(reg);

	return 0;

}

/*
int main(int argc, char* argv[]) {
	mime_parser mp;
	char * mime_type_str;
	char* exts[] = {"html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc", "html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc", "html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc"};
	int i;

	if(argc != 2) {
		printf("usage: %s <file_ext>\n", argv[0]);
		return 0;
	}

	mime_parser_init(&mp);

	for(i = 0; i < 30; i++) {
		parse_file_ext(&mp, exts[i], &mime_type_str);
		printf("%s Mime_type: '%s'\n", exts[i], mime_type_str);
		free(mime_type_str);
	}

	mime_parser_destroy(&mp);

	return 0;
}
*/